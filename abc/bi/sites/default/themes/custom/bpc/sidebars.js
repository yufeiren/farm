$(document).ready(function() {
  $('div.sidebarContent').each(function(div) {
	$('div.sidebarItem:first').addClass('first');
  })
  $('div.sidebarItem:only-child').addClass('first');
  $('div.print-email-rss ul.links li:last').addClass('last');
  $('div#primary ul.links li:last').addClass('last');	
  $('div.homeLeft #homeLeft-2col .greyBorder:last').addClass('last');
  $('div#detailedContent #columns .column:last').addClass('last');	
  $('div#detailedContent .articleList div.clearfix:last').addClass('last');	
  
  $('div#detailedContent ul.galleryList li:even').addClass('left');
  /*$("a.group").fancybox({ 'zoomSpeedIn': 300, 'zoomSpeedOut': 300, 'overlayShow': false });*/ 
  
  $('div#detailedContent ul.fancyList li:odd').addClass('alt');
  
  $('div#detailedContent div.projectLanding div.projectLogo-reflect:odd').addClass('projectLogo-reflectRight');
  
  $('div#detailedContent div.boardMember-founder:odd').addClass('boardMember-right');
  
  $('div#detailedContent div.boardMember-small:odd').addClass('boardMember-right');
  /* removing because no longer an odd number of board members
  $('div#detailedContent div.boardMember-small:last').addClass('lastBoardMember');*/		

});