$(document).ready(function(){
  var oneliner_items = ["\"Every difference of opinion is not a difference in principle.\" - Thomas Jefferson ","2009: Five Republican senators joined Democrats in passing the Lilly Ledbetter Fair Pay Act","The Bipartisan 2002 Campaign Reform Act passed through the efforts of Sens. John McCain (R-AZ) & Russ Feingold (D-WI)","2010: President Obama and congressional Republicans reached a deal to maintain unemployment benefits and extend tax cuts","Republicans support for Truman’s Post-WWII plans paved the way for the U.S. role in the UN, NATO, & Marshall Plan","At the height of the credit crisis in 2009, a mostly united Congress passed the Helping Families Save Their Homes Act","The 1983 Social Security Reform Act addressed both sides of the debate and was signed into law by President Reagan","The Credit Card Act of 2009, which mandated transparent credit practices, passed with 357 House and 90 Senate votes","The Edward M. Kennedy Serve America Act was passed with largely bipartisan support in the Congress","After the election of 1860, Abraham Lincoln appointed all three of his vanquished Republican rivals to his cabinet.","2010: Eight Republican senators joined Democrats in repealing the Don’t ask, don’t tell (DADT) policy","SCHIP provides health insurance to millions of children and was passed with unanimous support in the Senate","In 1996, President Bill Clinton worked with Republican majorities in the House and Senate to pass the Welfare Reform Act","Passage of No Child Left Behind was lead by Republicans Rep. Boehner & Sen. Gregg & Democrats Rep. Miller & Sen. Kennedy","58 Democrats and 14 Republicans backed the Food Safety Modernization Act that passed the Senate in November 2010","The Civil Rights Commission, a bipartisan commission formed by Pres. Eisenhower, enabled Civil Rights Acts of ‘60 & ’64","\"If I could not go to heaven but with a party, I would not go there at all.\" - Thomas Jefferson","“Let us not seek the Republican answer or the Democratic answer, but the right answer.\" - President John F. Kennedy","In the spring of 2011, the White House hosted a number of budget negotiations with congressional Republican leaders"];
  var current_item = 0;
  var onelinerUpdater = $.interval(function() {
    updateOneliner();
  }, 8000);
  
  function updateOneliner()
  {
    current_item++;
    if (oneliner_items[current_item] == undefined) current_item = 0;
    $('#onelinerContainer').fadeOut(300, function(){
      $('#onelinerItem').html(oneliner_items[current_item]);
    });
    $('#onelinerContainer').fadeIn();
  }
});