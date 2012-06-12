// @todo the only interval ajax we have so far. consider comet for the future
// and maybe use this as a fallback for servers that don't have node.js etc.

var courseInterval;

Drupal.behaviors.CourseNav = function() {
  courseInterval = setInterval('CourseFulfillmentCheck();', 2500);
}

function CourseFulfillmentCheck() {
  var url = Drupal.settings.activePath + '/ajax/nav';
  $.getJSON(url, {}, function (response) {
    if (response.complete) {
      $('#course-nav').html(response.content);
      clearInterval(courseInterval);
    }
  });
}
