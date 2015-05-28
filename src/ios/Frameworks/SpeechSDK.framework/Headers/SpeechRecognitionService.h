/*
* Copyright (c) Microsoft Corporation. All rights reserved.
*/

#pragma once

#import <Foundation/Foundation.h>

/**
Specifies the type of request to make to the speech recognition service.

<p>In Short Phrase mode, an utterance may only up to 15 seconds long. As data is sent to the service, the client receives multiple partial results and one
final multiple N-best recognition result. The N-best result represents the recognition service's best estimates of what was actually spoken.</p>

<p>In Long-form Dictation mode, an utterance may be only up to 2 minutes long. As data is sent to the service, the client receives multiple partial results and multiple
final phrase results based on where the recognition service thinks sentence pauses occur in the audio stream.</p>
*/
typedef NS_ENUM(NSUInteger, SpeechRecognitionMode) {
    /**
    Short phrase mode
    */
    SpeechRecognitionMode_ShortPhrase,

    /**
    Long-form dictation mode
    */
    SpeechRecognitionMode_LongDictation
};

/**
A base set of parameters to configure speech services.
*/
@interface Preferences : NSObject

/**
Gets or sets the speech recognition locale
*/
@property(nonatomic, readwrite, assign) NSString* Locale;

/**
Gets or sets the speech recognition service endpoint URI.
*/
@property(nonatomic, readwrite, assign) NSString* ServiceUri;

/**
The default microphone timeout in milliseconds. A value <= 0 disables automatic microphone shutoff.
*/
@property(nonatomic, readwrite, strong) NSNumber* MicrophoneTimeout;

@end


/**
A streamlined set of parameters for use for Adm and Reco situations to configure speech services.
*/
@interface AdmRecoOnlyPreferences : Preferences

/**
Gets or sets the Azure clientId to set.
*/
@property(nonatomic, readwrite, assign) NSString* ClientId;

/**
Gets or sets the Azure client secret to set.
*/
@property(nonatomic, readwrite, assign) NSString* ClientSecret;

/**
Gets or sets the LUIS application identifier. Once you have configured the LUIS service to create and publish
                  an intent model (see https://LUIS.ai) you will be given an Application ID guid.
                  Use that GUID here.
*/
@property(nonatomic, readwrite, assign) NSString* LuisAppId;

/**
Gets or sets the LUIS subscription identifier. Once you create a LUIS account (see https://LUIS.ai)
                           you will be given an Subscription ID. Use that secret here.
*/
@property(nonatomic, readwrite, assign) NSString* LuisSubscriptionId;

@end

/**
Status on how the recognition was produced. For example, was it produced as
a result of successful reco, or as a result of canceling the connection, etc...
*/
typedef NS_ENUM(NSInteger, RecognitionStatus) {
    /**
    No status
    */
    RecognitionStatus_None = 0,

    /**
    Response contains an intermediate result
    */
    RecognitionStatus_Intermediate = 100,

    /**
    The service received spoken audio but found one or more matches for the uttered phrase in its dictionary
    */
    RecognitionStatus_RecognitionSuccess = 200,

    /**
    The service received spoken audio but found no match for the uttered phrase in its dictionary
    */
    RecognitionStatus_NoMatch = 301,

    /**
    The service received audio input but received only silence before the maximum time to wait was exceeded
    */
    RecognitionStatus_InitialSilenceTimeout = 303,

    /**
    The service received audio input but received only background noise and did not recognize any spoken audio before the maximum time to wait was exceeded
    */
    RecognitionStatus_BabbleTimeout = 304,

    /**
    The service received audio input but did not recognize any of the configured key phrases before the maximum time to wait was exceeded
    */
    RecognitionStatus_HotWordMaximumTime = 305,

    /**
    Recognition was canceled by the client
    */
    RecognitionStatus_Cancelled = 201,

    /**
    There was an error during recognition
    */
    RecognitionStatus_RecognitionError = 500,

    /**
    In long-form dictation mode, the service exceeded the maximum time to wait for silence signalling the end of dictation
    */
    RecognitionStatus_DictationEndSilenceTimeout = 610,

    /**
    In long-form dictation mode, the service has finished all recognition and will not return any more final phrase results
    */
    RecognitionStatus_EndOfDictation = 612
};

/**
Indicates the confidence level ascribed by the speech recognition service to each N-best result.<p>The confidence level is based on a quantization of the internal confidence levels 
and takes into account post-recognition processing such as inverse text normalization (ITN), capitalization and punctuation.</p>
*/
typedef NS_ENUM(NSInteger, Confidence) {
    /**
    Indicates the speech recognition service has no confidence that the recognized phrase correctly represents the audio sample associated with the phrase.
    */
    SpeechRecoConfidence_None = -2,

    /**
    Indicates the speech recognition service has low confidence that the recognized phrase correctly represents the audio sample associated with the phrase.
    */
    SpeechRecoConfidence_Low = -1,

    /**
    Indicates the speech recognition service has normal confidence that the recognized phrase correctly represents the audio sample associated with the phrase.
    */
    SpeechRecoConfidence_Normal = 0,

    /**
    Indicates the speech recognition service has high confidence that the recognized phrase correctly represents the audio sample associated with the phrase.
    */
    SpeechRecoConfidence_High = 1
};

/**
A Confidence value for the recognized text and the recognized text with a set of processing types applied to it. This is one element of an N-best set.
*/
@interface RecognizedPhrase : NSObject

/**
The recognized phrase, without any post-processing.
<p>This form is optimal for use by applications that need the raw, unprocessed speech recognition results.</p>
*/
@property(nonatomic, readwrite, strong) NSString* LexicalForm;

/**
The recognized phrase with inverse text normalization, capitalization, punctuation and profanity masking applied.
<p>This form is optimal for use by applications that display the speech recognition results to a user.</p>
<p>Profanity is masked with asterisks after the initial character, e.g. "d***".
Inverse text normalization (ITN) has also been applied. An example of ITN is converting result text
from "go to fourth street" to "go to 4th st".</p>
*/
@property(nonatomic, readwrite, strong) NSString* DisplayText;

/**
The recognized phrase with inverse text normalization applied.
<p>This form is optimal for use by  applications that interpret the speech recognition results as commands or
which perform queries based on the recognized text.</p>
<p>Inverse text normalization (ITN) converts phrases like "one two three four" to a normalized form such as "1234".
Another example is converting result text from "go to fourth street" to "go to 4th st".</p>
*/
@property(nonatomic, readwrite, strong) NSString* InverseTextNormalizationResult;

/**
The recognized phrase with inverse text normalization *and* profanity masking applied, but not capitalization or punctuation.
<p>This form is optimal for use by applications that use the unmasked ITN results but also need to display
the command or query to the user.</p>
<p>Profanity is masked with asterisks after the initial character, e.g. "d***". 
Inverse text bormalization (ITN) is also been applied. An example of ITN is converting result text
from "go to fourth street" to "go to 4th st".</p>
*/
@property(nonatomic, readwrite, strong) NSString* MaskedInverseTextNormalizationResult;

/** 
A value representing high, medium or low confidence.
*/
@property(nonatomic, readwrite, assign) Confidence Confidence;

@end

/**
Defines a speech recognition result
*/
@interface RecognitionResult : NSObject

/**
The recgonition status from the service
*/
@property(nonatomic, readwrite, assign) RecognitionStatus RecognitionStatus;

/**
A set of N-best confidence values and recognized texts. The N-best result represents the recognition service's best estimates of what was actually spoken.
*/ 
@property(nonatomic, readwrite, strong) NSArray* RecognizedPhrase;

@end

/**
Defines a intent result
*/
@interface IntentResult : NSObject

/**
The intent uri (optional) from the recognition service.
*/
@property(nonatomic, readwrite, strong) NSURL* Url;

/**
The intent headers (optional) from the recognition service.
*/
@property(nonatomic, readwrite, strong) NSDictionary* Headers;

/**
The structured intent response body (optional) from the recognition service.<p>The response is a structured JSON string. Client applications must parse this string to access the JSON fields.</p>
*/
@property(nonatomic, readwrite, strong) NSString* Body;

@end

/**
The speech recognition callback protocol
*/
@protocol SpeechRecognitionProtocol

/**
Called when a partial response is received from the speech recognition service
@param partialResult The partial result received from the recognition service.
*/
-(void)onPartialResponseReceived:(NSString*)partialResult;

/**
Called when intent is received from the service
@param intent The intent response received from the recognition service.
*/
-(void)onIntentReceived:(IntentResult*)intent;

/**
Called when a final response is received from the service.
@param result The final phrase result received from the recognition service.
*/
-(void)onFinalResponseReceived:(RecognitionResult*)result;

/**
Called when a non-speech error occurs.<p>Non-speech errors include connection and authorization errors.</p>
@param errorMessage A optional error message describing the error.
@param errorCode The error code being signaled.
 */
-(void)onError:(NSString*)errorMessage withErrorCode:(int)errorCode;

/**
Called when the microphone recording status has changed.
@param recording A flag that indicates whether the microphone is currently recording.
*/
-(void)onMicrophoneStatus:(Boolean)recording;

/**
Called when the service signals suggestion text.  Note: The service must be configured in order to have this signaled.
@param suggestionText The returned suggestion text.
*/
@optional
-(void)onSuggestion:(NSString*)suggestionText;

@end

/**
Indicates the type of compression applied to the audio data
*/
typedef NS_ENUM(NSUInteger, AudioCompressionType) {
    
    /**
    PCM (uncompressed)
    */
    AudioCompressionType_PCM = 1,

    /**
    Siren7 (Siren SR compression)
    */
    AudioCompressionType_Siren7 = 654
};

/**
The SpeechAudioFormat contains information about how the audio data was recorded and stored including the type of compression used, number of channels, sample rate, bits per sample and other attributes.
*/
@interface SpeechAudioFormat : NSObject

/**
This value indicates how many bytes of audio data must be streamed to a D/A converter per second in order to play the audio.
*/
@property(nonatomic, readwrite, assign) int AverageBytesPerSecond;

/**
The number of significant bits in each audio sample. Usually 16 or 24.
*/
@property(nonatomic, readwrite, assign) short BitsPerSample;

/**
The number of data bytes per sample slice.
*/
@property(nonatomic, readwrite, assign) short BlockAlign;

/**
The number of separate audio signals in the audio data. A value of 1 means a mono signal, a value of 2 means a stereo signal.
*/
@property(nonatomic, readwrite, assign) short ChannelCount;

/**
<p>Specifies the type of compression used on the audio data as a short. Default value is PCM.</p>
*/
@property(nonatomic, readwrite, assign) AudioCompressionType EncodingFormat;

/**
Extra bytes for use to describe parameters to certain audio compression types.
<p>This field should be null for PCM.</p>
*/
@property(nonatomic, readwrite, strong) NSData* FormatSpecificData;

/**
Audio sample slices per second, where one slice includes all the channel samples. This value is unaffected by the number of channels.
*/
@property(nonatomic, readwrite, assign) int SamplesPerSecond;

/**
Iitializes a SpeechAudioFormat object
*/
-(id)init;

/**
Produces a SpeechAudioFormat for data encoded in Siren7.
<p>The data must be encoded in mono such that a 320 sample mono input frame produces a 40 bytes output frame.</p>
@param sampleRate The samples per second of the audio that was encoded (can only be 16000).
@return The created SpeechAudioFormat.
*/
+(SpeechAudioFormat*)createSiren7Format:(int)sampleRate;

/**
<p>Produces a SpeechAudioFormat for 16-bit PCM data.</p>

@param sampleRate The samples per second of the audio that was encoded (can only be 16000).
@return The created SpeechAudioFormat.
*/
+(SpeechAudioFormat*)create16BitPCMFormat:(int)sampleRate;

@end

/**
Base class for Conversational Understanding Sessions
*/
@interface ConversationBase : NSObject<SpeechRecognitionProtocol>
{
}

/**
Prepares the conversation with the desired preferences.
@param prefs A streamlined set of parameters for use for Reco situations to configure speech services.
@param delegate The recognition event callback.
*/
-(void)initWithPrefs:(Preferences*)prefs
        withProtocol:(id<SpeechRecognitionProtocol>)delegate;

/**
Refreshes the conversation context using the existing preferences.
*/
-(void)createConversation;

/**
Turns on the microphone and begins streaming data from the microphone to the speech recognition service.
<p>A built in Silence Detector is applied to the microphone data before it is streamed to the recognition service.</p>
 */
-(void)audioStart;

/**
Turns off th emicrophone and breaks the connection to the speech recognition service.
 */
-(void)audioStop;

/**
Sends a text query to the service if the service supports it.
@param textQuery The text query to send.
*/
-(void)sendText:(NSString*)textQuery;

/**
Sets the current geographic location to improve results.
@param latitude The geographic latitude coordinate.
@param longitude The geographic longitude coordinate.
*/
-(void)setLocationLatitude:(double)latitude
             withLongitude:(double)longitude;

@end

/**
The Azure Intelligent Services API client to perform speech recognition from acquired data (for example from a file or acquired audio source).
<p>Data is broken up into buffers and each buffer is sent to the speech recognition service.
No modification is done to the buffers; if silence detection is required, it must be performed in an external pre-processing pass over the data.
Audio data must be PCM, mono, 16-bit sample, with sample rate of 16000 Hz.</p>
<p>Returns only speech recognition results; intent detection is not performed.</p>
*/
@interface DataRecognitionClient : ConversationBase

/**
Initializes a speech recognition client that uses the buffered data as the input source.
<p>Callers are responsible for acquiring the audio data and writing that data to input streams. The data is split into buffers;
each buffer is sent to the speech recognition service.
No modification is done to the buffers; silence detection must be performed by an external algorithm before calling any methods to write data to the service.
The audio must be PCM, mono, 16-bit sample, with sample rate of 16000 Hz.</p>
<p>The recognition service returns only speech recognition results and does not perform intent detection.</p>
@param speechRecognitionMode The speech recognition mode. <p>In Short Phrase mode, the client receives one final multiple N-best choice result.
In Long-form Dictation mode, the client receives multiple final results, based on where the service thinks sentence pauses are.</p>
@param prefs A set of preferences used to configure the speech service.
@param wantIntent Set this parameter to false
@param delegate The speech recognition protocol callback
@return The speech recognition client
*/
-(id)initWithSpeechRecoParams:(SpeechRecognitionMode)speechRecognitionMode
                    withPrefs:(AdmRecoOnlyPreferences*)prefs
                   withIntent:(bool)wantIntent
                 withProtocol:(id<SpeechRecognitionProtocol>)delegate;

/**
Sends an audio format description to the service.
<p>If you are not sending up a audio file in wave format, but instead you have just
raw data, then before sending up any audio data you must first send up an
SpeechAudioFormat descriptor to describe the layout and format of your raw audio data.
The audio must be PCM, mono, 16-bit sample, with sample rate of 16000 Hz.
</p>
@param format Audio features that describe the encoding and sampling rate of the audio.
*/
-(void)sendAudioFormat:(SpeechAudioFormat*)audioFormat;

/**
Sends audio data to the service at a controlled rate.
<p>Collects and sends buffered audio to the speech recognition service. The audio must be PCM, mono, 16-bit sample, with
sample rate of 16000 Hz. For raw audio, i.e. audio data that is not in wave format (e.g. audio captured over Bluetooth), before
sending up any audio data with this method, you must call the sendAudioFormatWorker method to send a SpeechAudioFormat descriptor
that describes the layout and format of your raw audio data. This work is queued on a background worker thread.
</p>
<p>If you try to send audio data to the service too fast, this method will throttle the upload speech to match
the audio data rate specified in the SpeechAudioFormat for this audio stream.
</p>
@param buffer The data bytes of audio to send to the service.
@param actualAudioBytesInBuffer The actual number of audio bytes in the buffer.<p>The buffer size is allowed to be larger than the actual audio data in the buffer, so
this parameter states the size of the actual data in the buffer.</p>
*/
-(void)sendAudio:(NSData*)buffer
      withLength:(int)actualAudioBytesInBuffer;

/**
Notifies the service that the client is finished sending buffered audio.
<p>This work is queued onto a background worker.</p>
*/
-(void)endAudio;

/**
Waits the given number of seconds for a speech recognition response. *Calling this method blocks the current thread until the response is received or the wait time is exceeded*.
@param timeoutInSeconds The time to wait for a response, in seconds.
@return true if the final speech recognition result is sent from the speech recognition service or false if the timeout was reached.
*/
-(bool)waitForFinalResponse:(int)timeoutInSeconds;

@end


/**
<p>The Azure Intelligent Services API client to perform speech and intent recognition from a buffered data source (e.g. a file or Bluetooth audio source).</p>
<p>Data is broken up into buffers and each buffer is sent to the speech recognition service.
No modification is done to the buffers; if silence detection is required, it must be performed in an external pre-processing pass over the data.
The audio data must be PCM, mono, 16-bit sample, with sample rate of 16000 Hz.</p>
<p>Returns both speech recognition results *and* structured intent results (see https://LUIS.ai).</p>
*/
@interface DataRecognitionClientWithIntent : DataRecognitionClient

/**
Initializes a speech recognition client that uses the buffered data as the input source.
<p>Callers are responsible for acquiring the audio data and writing that data to input streams. The data is split into buffers; each buffer is sent to the speech recognition service.
No modification is done to the buffers; silence detection must be performed by an external algorithm before calling any methods to write data to the service. The service returns speech recognition results *and* structured intent results.
The audio must be PCM, mono, 16-bit sample, with sample rate of 16000 Hz.
</p>
<p>The service returns structured intent results in JSON form (see https://LUIS.ai).</p>
@param prefs A set of preferences used to configure the speech service.
@param delegate The speech recognition protocol callback
@return The speech and intent recognition client
*/
-(id)initWithSpeechRecoParams:(AdmRecoOnlyPreferences*)prefs
                 withProtocol:(id<SpeechRecognitionProtocol>)delegate;

@end

/**
<p>The Azure Intelligent Services speech recognition client for microphone input.</p>
<p>Voice input data is collected from the microphone and sent to the speech recognition service. A built-in Silence Detector
is applied to microphone data before it is sent to the recognition service.</p>
<p>Returns only speech recognition results on the input data; does not perform intent detection.</p>
*/
@interface MicrophoneRecognitionClient : ConversationBase
{
}

/**
Initializes a speech recognition client that uses the microphone as the input source.
<p>To initiate speech recognition, call the startMicAndRecognition method of this client. Once the microphone is
turned on, data from the microphone is sent to the speech recognition service.
A built-in Silence Detector is applied to the microphone data before it is sent to the recognition service.
The recognition service returns only speech recognition results and does not perform intent detection.
To terminate speech recognition and stop sending data to the service, call endMicAndRecognition.</p>
@param speechRecognitionMode The speech recognition mode. <p>In Short Phrase mode, the client receives one final multiple N-best choice result. In Long-form Dictation mode, the client receives multiple final results, based on where the server thinks sentence pauses are.</p>
@param prefs A set of preferences used to configure the speech service.
@param wantIntent Set this parameter to false
@param delegate The speech recognition protocol callback
@return The microphone speech recognition client
*/
-(id)initWithSpeechRecoParams:(SpeechRecognitionMode)speechRecognitionMode
                    withPrefs:(AdmRecoOnlyPreferences*)prefs
                   withIntent:(bool)wantIntent
                 withProtocol:(id<SpeechRecognitionProtocol>)delegate;

/**
Turns the microphone on and begins streaming data from the microphone to the speech recognition service.
<p>A built in Silence Detector is applied to the microphone data before it is sent to the service.</p>
*/
-(void)startMicAndRecognition;

/**
Turns the microphone off and breaks the connection to the speech recognition service.
*/
-(void)endMicAndRecognition;

/**
Waits the given number of seconds for a speech recognition response. *Calling this method blocks the current thread until the response is received or the wait time is exceeded*.
@param timeoutInSeconds The response wait timeout, in seconds
@return true if the final speech recognition result is sent from the speech recognition service or false if the timeout was reached.
*/
-(bool)waitForFinalResponse:(int)timeoutInSeconds;

@end

/**
<p>The Azure Intelligent Services speech recognition and intent client for microphone input.</p>
<p>Voice input data is collected from the microphone and sent to the speech recognition service. A built-in Silence Detector
is applied to microphone data before it is sent to the recognition service.</p>
<p>Returns speech recognition results *and* structured intent results.</p>
*/
@interface MicrophoneRecognitionClientWithIntent : MicrophoneRecognitionClient

/**
Initializes a speech recognition and intent client that uses the microphone as the input source.
<p>To initiate speech recognition, call the startMicAndRecognition method of this client. Once the microphone is turned on, data from the microphone is sent to the service.
A built-in Silence Detector is applied to the microphone data before it is sent to the recognition service. The service returns speech recognition results and structured intent results.
To terminate speech recognition and stop sending data to the service, call endMicAndRecognition.</p>
<p>The service returns structured intent results in JSON form (see https://LUIS.ai).</p>
@param prefs A set of preferences used to configure the service.
@param delegate The speech recognition protocol callback
@return The microphone recognition and intent client
*/
-(id)initWithSpeechRecoParams:(AdmRecoOnlyPreferences*)prefs
                 withProtocol:(id<SpeechRecognitionProtocol>)delegate;

@end

/**
Factory for creating clients for Azure Intelligent Services speech recognition. This factory can be used to create a client that interacts with the speech recognition service. There are four types of clients this factory can create.

DataRecognitionClient  This client is optimal for applications that require speech recognition with previously acquired data, for example from a file or Bluetooth audio source.
Data is broken up into buffers and each buffer is sent to the speech recognition service. No modification is done to the buffers; if silence detection is required, it must be performed in an external pre-processing pass over the data. Audio data must be PCM, mono, 16-bit sample, with sample rate of 16000 Hz. Returns speech recognition results.

DataRecognitionClientWithIntent  This client is optimal for applications that require speech recognition *and* intent detection with previously acquired data, for example from a file or Bluetooth audio source.
Data is broken up into buffers and each buffer is sent to the speech recognition service. No modification is done to the buffers; if silence detection is required, it must be performed in an external pre-processing pass over the data. Audio data must be PCM, mono, 16-bit sample, with sample rate of 16000 Hz. Returns speech recognition results and structured intent results. Intent results are returned in structured JSON form (see https://LUIS.ai)

MicrophoneRecognitionClient  This client is optimal for applications that require for speech recognition from microphone input.
When the microphone is turned on, audio data from the microphone is streamed to the speech recognition service. A built in Silence Detector is applied to the microphone data before it is sent to the recognition service. Returns speech recognition results.

MicrophoneRecognitionClientWithIntent  This client is optimal for applications that require for speech recognition *and* intent detection from microphone input.
When the microphone is turned on, audio data from the microphone is streamed to the speech recognition service. A built in Silence Detector is applied to the microphone data before it is sent to the recognition service. Returns speech recognition and intent results. Intent results are returned in structured JSON form (see https://LUIS.ai).
*/
@interface SpeechRecognitionServiceFactory : NSObject

/**
@return The version of the of the API you are currently using
*/
+(NSString*)getAPIVersion;

/**
Creates a DataRecognitionClient for speech recognition with acquired data, for example from a file or Bluetooth audio source.
<p>Data is broken up into buffers and each buffer is sent to the speech recognition service.
No modification is done to the buffers; if silence detection is required, it must be performed in an external pre-processing pass over the data.
Audio data must be PCM, mono, 16-bit sample, with sample rate of 16000 Hz.</p>
<p>The recognition service returns only speech recognition results and does not perform intent detection.</p>
@param speechRecognitionMode The speech recognition mode. <p>In Short Phrase mode, the client receives one final multiple N-best choice result.
In Long-form Dictation mode, the client receives multiple final results, based on where the service thinks sentence pauses are.</p>
@param language The language of the speech being recognized. The supported languages are:
* en-us: American English
* en-gb: British English
* de-de: German
* es-es: Spanish
* fr-fr: French
* it-it: Italian
* zh-cn: Mandarin Chinese
@param primaryOrSecondaryKey The primary or the secondary key.
You should periodically renew your key to prevent unauthorized use of your subscription. The recommended approach is to acquire two keys, a primary and a secondary and to rotate key usage between these two keys. While one key is disabled, the other key will still work, allowing your application to remain active while the disabled key is replaced.
@param delegate The protocol used to perform the callbacks/events upon during speech recognition.
@return The created DataRecognitionClient.
*/
+(DataRecognitionClient*)createDataClient:(SpeechRecognitionMode)speechRecognitionMode 
                             withLanguage:(NSString*)language
                                  withKey:(NSString*)primaryOrSecondaryKey
                             withProtocol:(id<SpeechRecognitionProtocol>)delegate;

/**
Creates a DataRecognitionClient with Acoustic Model Adaptation for speech recognition with acquired data, for example from a file or Bluetooth audio source.
<p>Data is broken up into buffers and each buffer is sent to the speech recognition service.
No modification is done to the buffers; if silence detection is required, it must be performed in an external pre-processing pass over the data.
Audio data must be PCM, mono, 16-bit sample, with sample rate of 16000 Hz.</p>
<p>The recognition service returns only speech recognition results and does not perform intent detection.</p>

@param speechRecognitionMode The speech recognition mode. <p>In Short Phrase mode, the client receives one final multiple N-best choice result.
In Long-form Dictation mode, the client receives multiple final results, based on where the service thinks sentence pauses are.</p>
@param language The language of the speech being recognized. The supported languages are:
* en-us: American English
* en-gb: British English
* de-de: German
* es-es: Spanish
* fr-fr: French
* it-it: Italian
* zh-cn: Mandarin Chinese
@param primaryOrSecondaryKey The primary or the secondary key.
You should periodically renew your key to prevent unauthorized use of your subscription. The recommended approach is to acquire two keys, a primary and a secondary and to rotate key usage between these two keys. While one key is disabled, the other key will still work, allowing your application to remain active while the disabled key is replaced.
@param delegate The protocol used to perform the callbacks/events upon during speech recognition.
@param url The endpoint with a Acoustic Model that you specially created with the Acoustic Model Specialization Service.
@return The created DataRecognitionClient.
*/
+(DataRecognitionClient*)createDataClient:(SpeechRecognitionMode)speechRecognitionMode 
                             withLanguage:(NSString*)language
                                  withKey:(NSString*)primaryOrSecondaryKey
                             withProtocol:(id<SpeechRecognitionProtocol>)delegate
                                  withUrl:(NSString*)url;

/**
Creates a DataRecognitionClientWithIntent for speech recognition *and* intent detection with previously acquired data, for example from a file or Bluetooth audio source.
<p>Data is broken up into buffers and each buffer is sent to the speech recognition service.
No modification is done to the buffers; if silence detection is required, it must be performed in an external pre-processing pass over the data.
Audio data must be PCM, mono, 16-bit sample, with sample rate of 16000 Hz. 
<p>Returns speech recognition results and structured intent results. Intent results are returned in structured JSON form (see https://LUIS.ai)</p>

@param language The language of the speech being recognized. The supported languages are:
* en-us: American English
* en-gb: British English
* de-de: German
* es-es: Spanish
* fr-fr: French
* it-it: Italian
* zh-cn: Mandarin Chinese
@param primaryOrSecondaryKey The primary or the secondary key.
You should periodically renew your key to prevent unauthorized use of your subscription. The recommended approach is to acquire two keys, a primary and a secondary and to rotate key usage between these two keys. While one key is disabled, the other key will still work, allowing your application to remain active while the disabled key is replaced.
@param luisAppID Once you have configured the LUIS service to create and publish an intent model (see https://LUIS.ai) you will be given an Application ID GUID. Use that GUID here.
@param luisSubscriptionID Once you create a LUIS account (see https://LUIS.ai) you will be given an Subscription ID. Use that secret here.
@param delegate The protocol used to perform the callbacks/events during speech recognition and intent detection.
@return The created DataRecognitionClientWithIntent.
*/
+(DataRecognitionClientWithIntent*)createDataClientWithIntent:(NSString*)language
                                                      withKey:(NSString*)primaryOrSecondaryKey
                                                withLUISAppID:(NSString*)luisAppID 
                                               withLUISSecret:(NSString*)luisSubscriptionID
                                                 withProtocol:(id<SpeechRecognitionProtocol>)delegate;

/**
Creates a DataRecognitionClientWithIntent with Acoustic Model Adaptation for speech recognition *and* intent detection with previously acquired data, for example from a file or Bluetooth audio source.
<p>Data is broken up into buffers and each buffer is sent to the speech recognition service.
No modification is done to the buffers; if silence detection is required, it must be performed in an external pre-processing pass over the data.
Audio data must be PCM, mono, 16-bit sample, with sample rate of 16000 Hz. 
<p>Returns speech recognition results and structured intent results. Intent results are returned in structured JSON form (see https://LUIS.ai)</p>
@param language The language of the speech being recognized. The supported languages are:
* en-us: American English
* en-gb: British English
* de-de: German
* es-es: Spanish
* fr-fr: French
* it-it: Italian
* zh-cn: Mandarin Chinese
@param primaryOrSecondaryKey The primary or the secondary key.
You should periodically renew your key to prevent unauthorized use of your subscription. The recommended approach is to acquire two keys, a primary and a secondary and to rotate key usage between these two keys. While one key is disabled, the other key will still work, allowing your application to remain active while the disabled key is replaced.
@param luisAppID Once you have configured the LUIS service to create and publish an intent model (see https://LUIS.ai) you will be given an Application ID GUID. Use that GUID here.
@param luisSubscriptionID Once you create a LUIS account (see https://LUIS.ai) you will be given an Subscription ID. Use that secret here.
@param delegate The protocol used to perform the callbacks/events during speech recognition and intent detection.
@param url The endpoint with a Acoustic Model that you specially created with the Acoustic Model Specialization Service.
@return the created DataRecognitionClientWithIntent.
*/
+(DataRecognitionClientWithIntent*)createDataClientWithIntent:(NSString*)language
                                                      withKey:(NSString*)primaryOrSecondaryKey
                                                withLUISAppID:(NSString*)luisAppID 
                                               withLUISSecret:(NSString*)luisSubscriptionID
                                                 withProtocol:(id<SpeechRecognitionProtocol>)delegate
                                                      withUrl:(NSString*)url;

/**
Creates a MicrophoneRecognitionClient that uses the microphone as the input source.
<p>To initiate speech recognition, call the startMicAndRecognition method of this client. Once the microphone is
turned on, data from the microphone is sent to the speech recognition service.
A built-in Silence Detector is applied to the microphone data before it is sent to the recognition service.
The recognition service returns only speech recognition results and does not perform intent detection.
To terminate speech recognition and stop sending data to the service, call endMicAndRecognition.</p>
@param speechRecognitionMode The speech recognition mode. <p>In Short Phrase mode, the client receives one final multiple N-best choice result. In Long-form Dictation mode, the client receives multiple final results, based on where the server thinks sentence pauses are.</p>
@param language The language of the speech being recognized. The supported languages are:
* en-us: American English
* en-gb: British English
* de-de: German
* es-es: Spanish
* fr-fr: French
* it-it: Italian
* zh-cn: Mandarin Chinese
@param primaryOrSecondaryKey The primary or the secondary key.
You should periodically renew your key to prevent unauthorized use of your subscription. The recommended approach is to acquire two keys, a primary and a secondary and to rotate key usage between these two keys. While one key is disabled, the other key will still work, allowing your application to remain active while the disabled key is replaced.
@param delegate The protocol used to perform the callbacks/events upon during speech recognition.
@return The created MicrophoneRecognitionClient.
*/
+(MicrophoneRecognitionClient*)createMicrophoneClient:(SpeechRecognitionMode)speechRecognitionMode
                                         withLanguage:(NSString*)language
                                              withKey:(NSString*)primaryOrSecondaryKey
                                         withProtocol:(id<SpeechRecognitionProtocol>)delegate;

/**
Creates a MicrophoneRecognitionClient with Acoustic Model Adaptation that uses the microphone as the input source.
<p>To initiate speech recognition, call the startMicAndRecognition method of this client. Once the microphone is
turned on, data from the microphone is sent to the speech recognition service.
A built-in Silence Detector is applied to the microphone data before it is sent to the recognition service.
The recognition service returns only speech recognition results and does not perform intent detection.
To terminate speech recognition and stop sending data to the service, call endMicAndRecognition.</p>
@param speechRecognitionMode The speech recognition mode. <p>In Short Phrase mode, the client receives one final multiple N-best choice result. In Long-form Dictation mode, the client receives multiple final results, based on where the server thinks sentence pauses are.</p>
@param language The language of the speech being recognized. The supported languages are:
* en-us: American English
* en-gb: British English
* de-de: German
* es-es: Spanish
* fr-fr: French
* it-it: Italian
* zh-cn: Mandarin Chinese
@param primaryOrSecondaryKey The primary or the secondary key.
You should periodically renew your key to prevent unauthorized use of your subscription. The recommended approach is to acquire two keys, a primary and a secondary and to rotate key usage between these two keys. While one key is disabled, the other key will still work, allowing your application to remain active while the disabled key is replaced.
@param delegate The protocol used to perform the callbacks/events upon during speech recognition.
@param url The endpoint with a Acoustic Model that you specially created with the Acoustic Model Specialization Service.
@return The created MicrophoneRecognitionClient
*/
+(MicrophoneRecognitionClient*)createMicrophoneClient:(SpeechRecognitionMode)speechRecognitionMode
                                         withLanguage:(NSString*)language
                                              withKey:(NSString*)primaryOrSecondaryKey
                                         withProtocol:(id<SpeechRecognitionProtocol>)delegate
                                              withUrl:(NSString*)url;

/**
Creates a MicrophoneRecognitionClientWithIntent that uses the microphone as the input source.
<p>To initiate speech recognition, call the startMicAndRecognition method of this client. Once the microphone is turned on, data from the microphone is sent to the service.
A built-in Silence Detector is applied to the microphone data before it is sent to the recognition service. The service returns speech recognition results and structured intent results.
To terminate speech recognition and stop sending data to the service, call endMicAndRecognition.</p>
<p>The service returns structured intent results in JSON form (see https://LUIS.ai).</p>

@param language The language of the speech being recognized. The supported languages are:
* en-us: American English
* en-gb: British English
* de-de: German
* es-es: Spanish
* fr-fr: French
* it-it: Italian
* zh-cn: Mandarin Chinese
@param primaryOrSecondaryKey The primary or the secondary key.
You should periodically renew your key to prevent unauthorized use of your subscription. The recommended approach is to acquire two keys, a primary and a secondary and to rotate key usage between these two keys. While one key is disabled, the other key will still work, allowing your application to remain active while the disabled key is replaced.
@param luisAppID Once you have configured the LUIS service to create and publish an intent model (see https://LUIS.ai) you will be given an Application ID GUID. Use that GUID here.
@param luisSubscriptionID Once you create a LUIS account (see https://LUIS.ai) you will be given an Subscription ID. Use that secret here.
@param delegate The protocol used to perform the callbacks/events during speech recognition and intent detection.
@return The created MicrophoneRecognitionClientWithIntent.
*/
+(MicrophoneRecognitionClientWithIntent*)createMicrophoneClientWithIntent:(NSString*)language
                                                                  withKey:(NSString*)primaryOrSecondaryKey
                                                            withLUISAppID:(NSString*)luisAppID
                                                           withLUISSecret:(NSString*)luisSubscriptionID
                                                             withProtocol:(id<SpeechRecognitionProtocol>)delegate;

/**
Creates a MicrophoneRecognitionClientWithIntent with Acoustic Model Adaptation that uses the microphone as the input source.
<p>To initiate speech recognition, call the startMicAndRecognition method of this client. Once the microphone is turned on, data from the microphone is sent to the service.
A built-in Silence Detector is applied to the microphone data before it is sent to the recognition service. The service returns speech recognition results and structured intent results.
To terminate speech recognition and stop sending data to the service, call endMicAndRecognition.</p>
<p>The service returns structured intent results in JSON form (see https://LUIS.ai).</p>

@param language The language of the speech being recognized. The supported languages are:
* en-us American English
* en-gb: British English
* de-de: German
* es-es: Spanish
* fr-fr: French
* it-it: Italian
* zh-cn: Mandarin Chinese
@param primaryOrSecondaryKey The primary or the secondary key.
You should periodically renew your key to prevent unauthorized use of your subscription. The recommended approach is to acquire two keys, a primary and a secondary and to rotate key usage between these two keys. While one key is disabled, the other key will still work, allowing your application to remain active while the disabled key is replaced.
@param luisAppID Once you have configured the LUIS service to create and publish an intent model (see https://LUIS.ai) you will be given an Application ID GUID. Use that GUID here.
@param luisSubscriptionID Once you create a LUIS account (see https://LUIS.ai) you will be given an Subscription ID. Use that secret here.
@param delegate The protocol used to perform the callbacks/events during speech recognition and intent detection.
@param url The endpoint with a Acoustic Model that you specially created with the Acoustic Model Specialization Service.
@return The created MicrophoneRecognitionClientWithIntent.
*/
+(MicrophoneRecognitionClientWithIntent*)createMicrophoneClientWithIntent:(NSString*)language
                                                                  withKey:(NSString*)primaryOrSecondaryKey
                                                            withLUISAppID:(NSString*)luisAppID
                                                           withLUISSecret:(NSString*)luisSubscriptionID
                                                             withProtocol:(id<SpeechRecognitionProtocol>)delegate
                                                                  withUrl:(NSString*)url;

@end
