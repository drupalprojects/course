<?php

/**
 * Implementation of hook_views_default_views().
 */
function course_views_default_views() {
  $views = array();

  $view_files = file_scan_directory(dirname(__FILE__) . '/default', '.*\.view\.inc');
  foreach ($view_files as $file) {
    include($file->filename);
    $views[$view->name] = $view;
  }


  return $views;
}
