<?php

/**
 * @file course blocks
 * @todo use context
 */

/**
 * Course outline block.
 */
function _course_block_outline() {
  if ($node = course_get_context()) {
    if ($output = course_get_outline($node)) {
      return array('subject' => t('Course outline'), 'content' => $output);
    }
  }
}

/**
 * Course navigation block. 
 */
function _course_block_navigation() {
  global $user;
  $node = course_get_context();
  if ($node) {
    $course = new Course($node, $user);
    foreach ($course->getObjects() as $courseObject) {
      if (!$courseObject->getFulfillment()->isComplete()) {
        $back = $previous->getTakeUrl();
      }
      $previous = $courseObject;
    }

    return array(
      'subject' => '',
      'content' => l('Back', $back) . l('Forward', $forward),
    );
  }
}
