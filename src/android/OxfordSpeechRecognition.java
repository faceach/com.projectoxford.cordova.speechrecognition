/*
Copyright (c) Microsoft Corporation
All rights reserved. 
MIT License
 
Permission is hereby granted, free of charge, to any person obtaining a copy of this 
software and associated documentation files (the "Software"), to deal in the Software 
without restriction, including without limitation the rights to use, copy, modify, merge, 
publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons 
to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or 
substantial portions of the Software.
THE SOFTWARE IS PROVIDED *AS IS*, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR 
PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE 
FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, 
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

package com.oxford.plugin.speechrecognition;

import java.util.ArrayList;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import org.apache.cordova.CallbackContext;
import org.apache.cordova.CordovaPlugin;
import org.apache.cordova.PluginResult;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.os.Bundle;
import android.util.Log;

import com.microsoft.ProjectOxford.Contract;
import com.microsoft.ProjectOxford.DataRecognitionClient;
import com.microsoft.ProjectOxford.DataRecognitionClientWithIntent;
import com.microsoft.ProjectOxford.ISpeechRecognitionServerEvents;
import com.microsoft.ProjectOxford.MicrophoneRecognitionClient;
import com.microsoft.ProjectOxford.MicrophoneRecognitionClientWithIntent;
import com.microsoft.ProjectOxford.RecognitionResult;
import com.microsoft.ProjectOxford.RecognitionStatus;
import com.microsoft.ProjectOxford.SpeechRecognitionMode;
import com.microsoft.ProjectOxford.SpeechRecognitionServiceFactory;

import java.io.IOException;
import java.io.InputStream;

public class OxfordSpeechRecognition extends CordovaPlugin implements ISpeechRecognitionServerEvents {

    public static final String ACTION_INIT = "init";
    public static final String ACTION_SPEECH_RECOGNIZE_START = "start";
    public static final String ACTION_SPEECH_RECOGNIZE_STOP = "stop";
    public static final String ACTION_SPEECH_RECOGNIZE_ABORT = "abort";

    private CallbackContext speechRecognizerCallbackContext;

    int m_waitSeconds = 0;
    DataRecognitionClient m_dataClient = null;
    MicrophoneRecognitionClient m_micClient = null;
    SpeechRecognitionMode m_recoMode;

    /*
    @Override
    public void initialize(CordovaInterface cordova, CordovaWebView webView) {
        super.initialize(cordova, webView);
    }
    */

    @Override
    public boolean execute(String action, JSONArray args, CallbackContext callbackContext) {

        Log.d("OxfordSpeechRecognition", "excute");

        // Dispatcher
        if (ACTION_INIT.equals(action)) {
            Log.d("OxfordSpeechRecognition", "initialize");
            // init
            initializeRecoClient(args);
        } else if (ACTION_SPEECH_RECOGNIZE_START.equals(action)) {
            Log.d("OxfordSpeechRecognition", "start - 1");
            speechRecognizerCallbackContext = callbackContext;
            // Speech recognition from the microphone.  The microphone is turned on and data from the microphone
            // is sent to the Speech Recognition Service.  A built in Silence Detector
            // is applied to the microphone data before it is sent to the recognition service.
            m_micClient.startMicAndRecognition();
            Log.d("OxfordSpeechRecognition", "start - 2");

            PluginResult pr = new PluginResult(PluginResult.Status.NO_RESULT);
            pr.setKeepCallback(true);
            callbackContext.sendPluginResult(pr);
        } else if (ACTION_SPEECH_RECOGNIZE_STOP.equals(action)) {
            stop(false);
        } else if (ACTION_SPEECH_RECOGNIZE_ABORT.equals(action)) {
            stop(true);
        } else {
            // Invalid action
            String res = "Unknown action: " + action;
            return false;
        }
        return true;
    }

    private void stop(boolean abort) {
        Log.d("OxfordSpeechRecognition", "end");

        boolean isReceivedResponse = false;
        if (m_micClient != null) {
            isReceivedResponse = m_micClient.waitForFinalResponse(m_waitSeconds);
            m_micClient.endMicAndRecognition();
            //m_micClient.dispose();
            //m_micClient = null;
        }
    }

    public void onPartialResponseReceived(final String response) {
        Log.d("OxfordSpeechRecognition", "partial");

        JSONObject event = new JSONObject();
        try {
            event.put("partial", response);
        } catch (JSONException e) {
            // this will never happen
        }
        PluginResult pr = new PluginResult(PluginResult.Status.OK, event);
        pr.setKeepCallback(true);
        speechRecognizerCallbackContext.sendPluginResult(pr);
    }

    public void onFinalResponseReceived(final RecognitionResult response) {
        Log.d("OxfordSpeechRecognition", "final");
        boolean isFinalDicationMessage = m_recoMode == SpeechRecognitionMode.LongDictation &&
                (response.RecognitionStatus == RecognitionStatus.EndOfDictation ||
                        response.RecognitionStatus == RecognitionStatus.DictationEndSilenceTimeout);
        if ((m_recoMode == SpeechRecognitionMode.ShortPhrase) || isFinalDicationMessage) {
            // we got the final result, so it we can end the mic reco.  No need to do this
            // for dataReco, since we already called endAudio() on it as soon as we were done
            // sending all the data.
            m_micClient.endMicAndRecognition();
        }

        if ((m_recoMode == SpeechRecognitionMode.ShortPhrase) || isFinalDicationMessage) {
            //speechRecognizerCallbackContext.sendPluginResult(pr); 
        }

        JSONObject event = new JSONObject();
        String result = "";
        if (!isFinalDicationMessage) {
            //for (int i = 0; i < response.Results.length; i++) {
            //response.Results[i].DisplayText;
            //}
            result = response.Results[0].DisplayText;
        }
        try {
            event.put("result", result);
        } catch (JSONException e) {
            // this will never happen
        }
        PluginResult pr = new PluginResult(PluginResult.Status.OK, event);
        pr.setKeepCallback(true);
        speechRecognizerCallbackContext.sendPluginResult(pr);
    }

    /**
     * Invoked when the audio recording state has changed.
     *
     * @param recording The current recording state
     */
    public void onAudioEvent(boolean recording) {
        if (!recording) {
            m_micClient.endMicAndRecognition();
        }
    }

    public void onError(final int errorCode, final String response) {
    }

    /**
     * Called when a final response is received and its intent is parsed
     */
    public void onIntentReceived(final String payload) {
    }

    void initializeRecoClient(JSONArray args) {
        try {
            // Set the mode and microphone flag to your liking   
            m_recoMode = SpeechRecognitionMode.ShortPhrase;
            m_waitSeconds = m_recoMode == SpeechRecognitionMode.ShortPhrase ? 20 : 200;

            String language = args.getString(0);
            String primaryOrSecondaryKey = args.getString(1);
            String luisAppID = args.getString(2);
            String luisSubscriptionID = args.getString(3);

            if (null == m_micClient) {
                m_micClient = SpeechRecognitionServiceFactory.createMicrophoneClient(m_recoMode,
                        language,
                        this,
                        primaryOrSecondaryKey);
            }
        } catch (JSONException e) {
            // this will never happen
        }
    }
}
