/* 
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
var my = require('myclass'),
log4js = require("log4js"), log = log4js.getLogger("Utils");

var Utils = my.Class({
});
Utils.DEFAULT_ENCODING = 'UTF-8';
Utils.DEBUG_MODE = true;

Utils.timeSeconds = function() {
    var time = process.hrtime();

    return time[0] + (time[1] * Utils.NANO_TO_SECONDS);
};

Utils.fill = function(object, value){
    var key;
    
    for(key in object){
        object[key] = value;
    }
};

Utils.clone = function(obj){
    var key, newObj = {};
    
    for(key in obj){
        newObj[key] = obj[key];
    }
    
    return newObj;
};

Utils.NANO_TO_SECONDS = 1.0 / (1000 * 1000 * 1000);

module.exports = Utils;