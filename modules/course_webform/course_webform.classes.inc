<?php

/**
 * Parent class for webform course tracking.
 */
class CourseObjectWebform extends CourseObjectNode {

  public function create($node = NULL) {
    $webform = new stdClass;
    $webform->type = 'webform';
    $webform->title = $this->getTitle();
    $webform->uid = $this->user->uid;
    node_object_prepare($webform);
    node_save($webform);
    $this->setNode($webform);
  }

  public function getWarnings() {
    $warnings = parent::getWarnings();

    if ($this->getInstanceId()) {
      if (!$this->node->webform['components']) {
        $warnings[] = t('The Webform has no questions. Please !link.', array('!link' => l('add questions', "node/{$this->getInstanceId()}/webform")));
      }
    }

    return $warnings;
  }

  public function getReports() {
    $reports = parent::getReports();
    $reports += array(
      'submissions' => array(
        'title' => t('Submissions'),
      ),
      'analysis' => array(
        'title' => t('Analysis'),
      ),
      'download' => array(
        'title' => t('Download'),
      ),
    );
    return $reports;
  }

  public function getReport($key) {
    module_load_include('inc', 'webform', 'includes/webform.report');
    switch ($key) {
      case 'submissions':
        return array(
          'title' => t('Webform results'),
          'content' => webform_results_submissions($this->node, FALSE, 50),
        );
      case 'analysis':
        return array(
          'title' => t('Webform results'),
          'content' => webform_results_analysis($this->node),
        );
      case 'download':
        $out = drupal_get_form('webform_results_download_form', $this->node);
        return array(
          'title' => t('Webform results'),
          'content' => drupal_render($out),
        );
    }
    return parent::getReport($key);
  }

  function getNodeTypes() {
    return array('webform');
  }

  function getCloneAbility() {
    return TRUE;
  }

  function getOptionsSummary() {
    $summary = parent::getOptionsSummary();
    if ($this->getInstanceId()) {
      $summary['questions'] = l('Edit questions', "node/{$this->getInstanceId()}/webform");
    }
    return $summary;
  }

}
