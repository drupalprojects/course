<?php

/**
 * Parent class for webform course tracking.
 */
class CourseObjectWebform extends CourseObjectNode {

  public function create() {
    global $user;
    $webform = new stdClass;
    $webform->type = 'webform';
    $webform->title = $this->getTitle();
    $webform->uid = $user->uid;
    node_save($webform);
    $this->setInstanceId($webform->nid);
  }

  public function getWarnings() {
    $warnings = parent::getWarnings();

    if (!$this->node->webform['components']) {
      $warnings[] = t('The Webform %w has no questions.', array('%w' => $this->getTitle()));
    }

    return $warnings;
  }
}
