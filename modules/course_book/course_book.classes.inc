<?php

class CourseObjectBook extends CourseObjectNode {
  function getNodeTypes() {
    return array('book');
  }

  function optionsDefinition() {
    $defaults = parent::optionsDefinition();
    $defaults['book_tracking'] = 'all';
    return $defaults;
  }

  function optionsForm(&$form, &$form_state) {
    $config = $this->optionsDefinition();
    parent::optionsForm($form, $form_state);
    $form['book_tracking'] = array(
      '#title' => t('Completion criteria'),
      '#type' => 'select',
      '#options' => array(
        'one' => 'View any page',
        'all' => 'View all pages',
      ),
      '#default_value' => $config['book_tracking'],
    );
    return $form;
  }

  /**
   * Grade (track) the book based on the fulfillment data.
   */
  function grade() {
    $mlids = array_keys(book_toc($this->node->nid, array(), 99));
    $viewed = array_keys(array_filter($this->getFulfillment()->getOption('book_fulfillment')));
    if ($this->getOption('book_tracking') == 'all') {
      if (!array_diff($mlids, $viewed)) {
        $this->getFulfillment()->setComplete(1)->save();
      }
    }
    elseif ($this->getOption('book_tracking') == 'one') {
      $this->getFulfillment()->setComplete(1)->save();
    }
  }
}
