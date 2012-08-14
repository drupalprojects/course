<?php

/**
 * Dummy example course object.
 */
class CourseObjectTest extends CourseObject {

}

/**
 * Dummy example course object.
 */
class CourseObjectGradedTest extends CourseObject {

  function isGraded() {
    return TRUE;
  }

}
