<?php

/**
 * Theme the course outline.
 */
function theme_course_outline($variables) {
  $output = '';
  $output .= '<div class="course-outline">';
  $output .= '<span class="trigger"></span>';
  $output .= '<h4 class="course-title">' . $variables['node']->title . '</h4>';
  $output .= theme('item_list', array('items' => $variables['items']));
  $output .= '</div>';
  return $output;
}
