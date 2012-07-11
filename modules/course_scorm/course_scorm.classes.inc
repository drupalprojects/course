<?php

class CourseObjectScorm extends CourseObjectNode {

  public function create() {
    global $user;
    if ($this->getInstanceId()) {
      $scorm_node = node_load($this->getInstanceId());
    }
    else {
      $scorm_node = new stdClass;
    }
    $scorm_node->type = $this->getComponent();
    $scorm_node->title = $this->getTitle();
    $scorm_node->uid = $user->uid;
    node_save($scorm_node);
    $this->setInstanceId($scorm_node->nid);
  }

  public function delete($course) {

  }

  function getNodeTypes() {
    return array();
  }

}
