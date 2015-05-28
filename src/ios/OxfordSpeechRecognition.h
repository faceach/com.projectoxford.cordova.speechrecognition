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

#import <Cordova/CDV.h>
#import "SpeechSDK/SpeechRecognitionService.h"

/**
* The Main App
*/
@interface OxfordSpeechRecognition : CDVPlugin<SpeechRecognitionProtocol>
{
    MicrophoneRecognitionClient* micClient;
    SpeechRecognitionMode recoMode;
    int waitSeconds;
}

@property (nonatomic,strong) CDVInvokedUrlCommand * command;
@property (nonatomic,strong) CDVPluginResult* pluginResult;

/**
* Called when a partial response is received; 
*/
-(void)onPartialResponseReceived:(NSString*)response;

/**
* Called when a final response is received. 
*/
-(void)onFinalResponseReceived:(RecognitionResult*)result;

/**
* Called when an intent is parsed and received. 
*/
-(void)onIntentReceived:(NSString*)payload;

/**
* Called when an error is received.
*/
-(void)onError:(NSString*)errorMessage withErrorCode:(int)errorCode;

/**
* Event fired when the microphone recording status has changed.
* @param recording Whether the microphone is currently recording.
*/
-(void)onMicrophoneStatus:(Boolean)recording;

@end

