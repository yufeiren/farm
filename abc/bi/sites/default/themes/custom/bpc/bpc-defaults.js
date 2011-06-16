var main_dropdowns = function () {
	var links = $('#primary ul.menu > li');
	
	links.each(function (i) {
		$(this).hover(function () {
			$(this).addClass('hovered');
		}, function () {
			$(this).removeClass('hovered');
		});
		
		$(this).find('ul ul').remove();
	});
};


$(window).load(function(){
	/*
		Fix flash embeds to show behind fancybox
	*/
	$("object").attr("wmode", "opaque").wrap('<div>');
	$('object').each(function() {
		if($(this).parent()[0]['tagName'] !='OBJECT') {
			$(this).attr("wmode", "opaque").wrap('<div>');
		}
	});
});


$(document).ready(function() {
	main_dropdowns();
	

	
	// Remove the right border on any tab to the left of the active tab
	var previousTab;
	if($('#primary .active').length != 0) {
		previousTab = $('#primary li.active').prev();
	} else {
		previousTab = $('#primary .active-trail').prev();
	}
	$(previousTab).css({
		'background':'none'
	});

  //force home page middle blocks to same size
  box1 = $('#featuredEvent').parent();
  box2 = $('#latestNews').parent();
  if (box1.height() > box2.height())
    box2.height(box1.height());
  else
    box1.height(box2.height());

});



