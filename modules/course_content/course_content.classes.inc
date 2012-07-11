<?php

class CourseObjectContent extends CourseObjectNode {
  function getNodeTypes() {
    $handlers = course_content_course_handlers();
    return array_keys($handlers['object']);
  }
}
