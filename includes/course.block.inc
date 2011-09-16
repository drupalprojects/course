<?php

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
