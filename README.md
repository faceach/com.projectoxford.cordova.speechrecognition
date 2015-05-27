Cordova Plugin for Microsoft Oxford Speech Recognition
==============
Project Oxford Speech Documentation URL: https://www.projectoxford.ai/doc/speech/overview

> **Note:**
> - IOS version is on develop.

Code example
------------
```
    var recognition = new OxfordSpeechRecognition({
        "lang": "zh-cn",
        "primaryKey": "your subscription key"
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

Copyright (c) Microsoft Corporation
All rights reserved. 
