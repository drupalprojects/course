<?php

class CourseObjectUbercart extends CourseObjectNode {

  function getTakeType() {
    return 'content';
  }

  function take() {
    if ($this->getFulfillment()->isComplete()) {
      return "You've already purchased this activity.";
    }
    else {
      $atc = drupal_get_form('uc_product_add_to_cart_form', $this->node);
      if (!$atc) {
        // The add to cart form was hidden?
        return "Something isn't right...";
      }
      else {
        return $atc;
      }
    }
  }

  public function optionsForm(&$form, &$form_state) {
    parent::optionsForm($form, $form_state);

    $form['instance']['#title'] = t('Product to check');
    $form['instance']['#description'] .= '</br>' . t('Check a different ubercart-enabled product for purchase. Defaults to this course.');
  }

  /**
   * Set a specialized instance ID.
   *
   * Either the course node ID, or another ubercart node ID.
   */
  public function optionsSubmit(&$form, &$form_state) {
    if (!$form_state['values']['instance']) {
      $form_state['values']['instance'] = $this->getCourseNid();
    }
    parent::optionsSubmit($form, $form_state);
  }

  /**
   * Don't create new products.
   */
  public function create() {
    if (!$this->getInstanceId()) {
      $this->setOption('instance', $this->getCourseNid());
    }
  }
}
