<?php

class CourseObjectManual extends CourseObject {
  public function getStatus($type = 'short') {
      $grade = $this->getFulfillment()->getGrade();
      if ($grade === NULL) {
        return 'Your instructor has not given you a pass/fail grade yet.';
      }

      if ($this->getFulfillment()->isComplete()) {
        return 'Your instructor has marked you as passed.';
      }
      else {
        return 'Your instructor has marked you as failed.';
      }
   }
}
