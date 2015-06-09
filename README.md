Cordova Plugin for Microsoft Project Oxford Speech Recognition
==============
Project Oxford Speech Documentation URL: https://www.projectoxford.ai/doc/speech/overview

> **Note:**
> - IOS version is on develop.

Installation
------------
```
cordova plugin add https://github.com/faceach/com.projectoxford.cordova.speechrecognition.git
```

Code example
------------
```
    var recognition = new OxfordSpeechRecognition({
        "lang": "zh-cn",
        "primaryKey": "<your subscription key>"
    });
    recognition.onresult = function(event) {
        if (event.result) {
            //...
        } else if (event.partial) {
            //...
        }
        
    };
    recognition.start();
```

© 2015 Microsoft