<?php

/**
 * Dummy example course object.
 */
class CourseObjectTest extends CourseObject {

  public static function getMaxOccurences() {
    return 3;
  }

  public function take() {
    return t('I am a test course object with the title !title', array('!title' => $this->getOption('title')));
  }

}

/**
 * Dummy example course object.
 */
class CourseObjectGradedTest extends CourseObject {

  public function isGraded() {
    return TRUE;
  }

}
