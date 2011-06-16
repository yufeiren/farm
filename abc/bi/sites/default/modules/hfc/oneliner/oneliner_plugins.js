//fade-toggle
jQuery.fn.fadeToggle = function(speed, easing, callback) {
	 return this.animate({opacity: 'toggle'}, speed, easing, callback);
};


//interval/timer plugin http://plugins.jquery.com/project/timeout_interval_idle
(function($) {

	if (typeof $.timeout != "undefined") return; 

	$.extend({
	  timeout : function (func,delay) {
			// init
			if (typeof $.timeout.count == "undefined") $.timeout.count = 0; 	
			if (typeof $.timeout.funcs == "undefined") $.timeout.funcs = new Array(); 
			// set timeout
			if (typeof func =='string') return setTimeout(func, delay); 
			if (typeof func =='function') {
				$.timeout.count++;
				$.timeout.funcs[$.timeout.count] = func;
				return setTimeout("$.timeout.funcs['"+$.timeout.count+"']();", delay);
			}
		},
	  interval : function (func,delay) {
			// init
			if (typeof $.interval.count == "undefined") $.interval.count = 0; 	
			if (typeof $.interval.funcs == "undefined") $.interval.funcs = new Array(); 
			// set interval
			if (typeof func =='string') return setInterval(func, delay); 
			if (typeof func =='function') {
				$.interval.count++;
				$.interval.funcs[$.interval.count] = func;
				return setInterval("$.interval.funcs['"+$.interval.count+"']();", delay);
			}
		},
	  idle : function (func,delay) {
			// init
			if (typeof $.idle.lasttimeout == "undefined") $.idle.lasttimeout = null;
			if (typeof $.idle.lastfunc == "undefined") $.idle.lastfunc = null;
			// set idle timeout
			if ($.idle.timeout) { clearTimeout($.idle.timeout); $.idle.timeout = null; $.idle.lastfunc = null; }
			if (typeof(func)=='string') { 
				$.idle.timeout = setTimeout(func, delay); 
				return $.idle.timeout;
			}		
			if (typeof(func)=='function') { 
				$.idle.lastfunc = func;
				$.idle.timeout = setTimeout("$.idle.lastfunc();", delay);
				return $.idle.timeout;
			}
		},
	  clear : function (countdown) {
		clearInterval(countdown);
		clearTimeout(countdown);
	  }	
	});
})(jQuery);



function in_array(needle, haystack, strict) {
	// http://kevin.vanzonneveld.net
	// +   original by: Kevin van Zonneveld (http://kevin.vanzonneveld.net)
	// *     example 1: in_array('van', ['Kevin', 'van', 'Zonneveld']);
	// *     returns 1: true
	
	var found = false, key, strict = !!strict;
	
	for (key in haystack) {
		if ((strict && haystack[key] === needle) || (!strict && haystack[key] == needle)) {
			found = true;
			break;
		}
	}
	return found;
}