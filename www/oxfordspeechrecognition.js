var exec = require("cordova/exec");

var OxfordSpeechRecognition = function(args) {
    var lang = args.lang || "en-us";
    var primaryKey = args.primaryKey || "yourPrimaryOrSecondaryKey";
    var luisAppID = args.luisAppID || "yourLuisAppID";
    var luisSubscriptionID = args.luisSubscriptionID || "yourLuisSubscriptionID";

    this.onresult = null;
    this.onend = null;

    exec(function() {
        console.log("initialized");
    }, function(e) {
        console.log("error: " + e);
    }, "OxfordSpeechRecognition", "init", [lang, primaryKey, luisAppID, luisSubscriptionID]);
};

OxfordSpeechRecognition.prototype.start = function() {
    var that = this;
    var successCallback = function(event) {
        that.onresult(event);
    };
    var errorCallback = function(err) {
        if (typeof that.onerror === "function") {
            that.onerror(err);
        }
    };

    exec(successCallback, errorCallback, "OxfordSpeechRecognition", "start", []);
};

OxfordSpeechRecognition.prototype.stop = function() {
    exec(null, null, "OxfordSpeechRecognition", "stop", []);
};

OxfordSpeechRecognition.prototype.abort = function() {
    exec(null, null, "OxfordSpeechRecognition", "abort", []);
};

module.exports = OxfordSpeechRecognition;