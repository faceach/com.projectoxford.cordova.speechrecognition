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

#import "OxfordSpeechRecognition.h"
#import <Cordova/CDV.h>
#import <AVFoundation/AVAudioSession.h>

@implementation OxfordSpeechRecognition

- (void) init:(CDVInvokedUrlCommand*)command {
    NSLog(@"OxfordSR - Init");

    // Setup the type of reco we want
    recoMode = SpeechRecognitionMode_ShortPhrase;
    
    waitSeconds = recoMode == SpeechRecognitionMode_ShortPhrase ? 20 : 200;
    // In the case of microphone use, setup things so microphone can be turned on later.
    [self activateAudioSession];


    NSString* language = [[command arguments] objectAtIndex:0];
    
    NSString* primaryOrSecondaryKey = [[command arguments] objectAtIndex:1];
    NSString* luisAppID = [[command arguments] objectAtIndex:2];
    NSString* luisSubscriptionID = [[command arguments] objectAtIndex:3];
    
    micClient = [SpeechRecognitionServiceFactory createMicrophoneClient:(recoMode)
                                                           withLanguage:(language)
                                                                withKey:(primaryOrSecondaryKey)
                                                           withProtocol:(self)];
}

/**
* In the case of microphone use, setup things so microphone can be turned on later.
*/
-(void)activateAudioSession
{
    NSError * err = nil;
    AVAudioSession* session = [AVAudioSession sharedInstance];

    if (![session setActive:YES error:&err])
    {
        NSLog(@"ERROR INITIALIZING AUDIO SESSION! %@\n", [err description]);
    }
    else
    {
        if ( ![session setCategory:AVAudioSessionCategoryPlayAndRecord
                             error:&err] )
        {
            NSLog(@"couldn't set audio category! %@", err);
        }
    }
    
    if ( ![session overrideOutputAudioPort:AVAudioSessionPortOverrideSpeaker
                                     error:&err] )
    {
        NSLog(@"couldn't set audio category! %@", err);
    }
        
    if ( ![session setActive:YES error:&err] )
    {
        NSLog(@"AudioSessionSetActive (true) failed %@", err);
    }
}

/**
* Called when a partial response is received. 
*/
-(void)onPartialResponseReceived:(NSString*) response
{
    NSLog(@"OxfordSR - Partial");
    dispatch_async(dispatch_get_main_queue(), ^{
        NSLog(@"OxfordSR - Partial %@", response);

        NSMutableDictionary * event = [[NSMutableDictionary alloc]init];
        [event setValue:response forKey:@"partial"];
        
        self.pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK messageAsDictionary:event];
        [self.pluginResult setKeepCallbackAsBool:YES];
        [self.commandDelegate sendPluginResult:self.pluginResult callbackId:self.command.callbackId];
    });
}

/**
* Called when an intent is parsed and received. 
*/
-(void)onIntentReceived:(IntentResult*) result
{
    dispatch_async(dispatch_get_main_queue(), ^{
    });
}

/**
* Called when a final response is received. 
*/
-(void)onFinalResponseReceived:(RecognitionResult*)response
{
    NSLog(@"OxfordSR - Final");
    bool isFinalDicationMessage = recoMode == SpeechRecognitionMode_LongDictation &&
                                                (response.RecognitionStatus == RecognitionStatus_EndOfDictation ||
                                                 response.RecognitionStatus == RecognitionStatus_DictationEndSilenceTimeout);
    if ((recoMode == SpeechRecognitionMode_ShortPhrase) || isFinalDicationMessage) {
        // we got the fial result, so we can end the mic reco.  No need to do this for dataReco, since
        // we already called endAudio on it as soon as we were don sending all the data.
        [micClient endMicAndRecognition];
    }

    if ((recoMode == SpeechRecognitionMode_ShortPhrase) || isFinalDicationMessage) {
    }
    
    if (!isFinalDicationMessage) {
        dispatch_async(dispatch_get_main_queue(), ^{
            RecognizedPhrase* phrase = response.RecognizedPhrase[0];
            NSString* result = phrase.DisplayText;
            NSLog(@"OxfordSR - Final %@", result);

            NSMutableDictionary * event = [[NSMutableDictionary alloc]init];
            [event setValue:result forKey:@"result"];
            
            self.pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK messageAsDictionary:event];
            [self.pluginResult setKeepCallbackAsBool:YES];
            [self.commandDelegate sendPluginResult:self.pluginResult callbackId:self.command.callbackId];
        });
    }
    
}

/**
* Called when an error is received.
*/
-(void)onError:(NSString*)errorMessage withErrorCode:(int)errorCode
{
    dispatch_async(dispatch_get_main_queue(), ^{
    });
}

/**
* Event fired when the microphone recording status has changed.
* @param recording Whether the microphone is currently recording.
*/
-(void)onMicrophoneStatus:(Boolean)recording
{
    if (!recording) {
        [micClient endMicAndRecognition];
    }

    dispatch_async(dispatch_get_main_queue(), ^{
    });
}

/**
* Take enum value and produce NSString
*/
NSString* ConvertSpeechRecoConfidenceEnumToString(Confidence confidence)
{
    switch (confidence) {
        case SpeechRecoConfidence_None:
            return @"None";

        case SpeechRecoConfidence_Low:
            return @"Low";

        case SpeechRecoConfidence_Normal:
            return @"Normal";

        case SpeechRecoConfidence_High:
            return @"High";
    }
}

/**
* Action for pressing the "Start" button
*/
- (void) start:(CDVInvokedUrlCommand*)command
{
    NSLog(@"OxfordSR - Start");
    self.command = command;
    [micClient startMicAndRecognition];

    NSMutableDictionary * event = [[NSMutableDictionary alloc]init];
    [event setValue:@"start" forKey:@"type"];
    self.pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK messageAsDictionary:event];
    [self.pluginResult setKeepCallbackAsBool:YES];
    [self.commandDelegate sendPluginResult:self.pluginResult callbackId:self.command.callbackId];
}

/**
* Action for pressing the "ShowFinalResponse" button
*/
- (void) stop:(CDVInvokedUrlCommand*)command
{
    NSLog(@"OxfordSR - Stop");
    bool isRecieivedResponse = false;
    
    if (micClient != nil) {
        isRecieivedResponse = [micClient waitForFinalResponse:(waitSeconds)];
        [micClient endMicAndRecognition];
        //[micClient finalize];
    }

}

@end
