<?php

/**
 * An object that holds CourseObjects and tracker functions?
 */
class Course {

  private $node;
  private $user;
  private $courseObjects;
  private $active;
  private $next;
  private $prev;

  public function __construct($node, $user) {
    if (is_object($node)) {
      $this->node = $node;
    }
    else {
      $this->node = node_load($node);
    }

    if (is_object($user)) {
      $this->user = $user;
    }
    else {
      $this->user = user_load($user);
    }

    $this->getObjects();
    $this->setActive();
  }

  public function setActive($id = NULL) {
    if (!$id) {
      $id = $_SESSION['course'][$this->node->nid]['taking']['active'];
    }

    foreach ($this->getObjects() as $courseObject) {
      if ($id == $courseObject->getId()) {
        // Active - save old, store next.
        if ($old) {
          $this->prev = $old;
        }

        $storeNext = TRUE;
        $this->active = $courseObject;
      }
      else if ($storeNext) {
        $this->next = clone $courseObject;
        $storeNext = FALSE;
      }

      $old = clone $courseObject;
    }
  }

  public function getActive() {
    return $this->active;
  }

  public function getNext() {
    return $this->next;
  }

  public function getPrev() {
    return $this->prev;
  }

  /**
   * Track the course (scan requirements, update the progress, completion etc)
   */
  public function track() {
    $sql = "select * from {course_outline} cr
    left join {course_outline_fulfillment} rf on (rf.uid = %d and rf.snid = cr.snid)
    where nid = %d and enabled
    order by weight asc";
    $result = db_query($sql, $this->user->uid, $this->node->nid, $this->user->uid);
    $report = new stdClass();
    while ($row = db_fetch_object($result)) {
      if (!$prev) {
        $report->section_name = $row->title;
      }

      // Count required objects.
      $required += $row->required;

      // Count completed required objects.
      $required_complete += ($row->required && $row->complete);

      if (!$row->complete && $prev->complete) {
        $report->section_name = $row->title;
      }

      $prev = clone $row;
    }

    if ($required_complete >= $required) {
      // Course requirements have been met.
      $report->section = 'complete';
      $report->section_name = 'Complete';
      $report->complete = TRUE;
    }

    $report->nid = $this->node->nid;
    $report->uid = $this->user->uid;
    course_report_save($report);
  }

  /**
   * Get the course objects in this course.
   *
   * @return array
   *   An array of course objects.
   */
  public function getObjects() {
    global $user;

    if (!$this->courseObjects) {
      $sql = 'SELECT * FROM {course_outline} co
        WHERE nid = %d
        ORDER BY weight ASC';
      $result = db_query($sql, $this->node->nid);

      while ($row = db_fetch_object($result)) {
        $courseObject = course_get_course_object($row, $user);
        $this->courseObjects[] = $courseObject;
      }
    }

    return $this->courseObjects;
  }

}
