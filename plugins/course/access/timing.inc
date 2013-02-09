<?php

$plugin = array(
  'title' => t('Date-based access'),
  'description' => t('Controls access by release and expiration dates'),
  'handler' => array(
    'class' => 'CourseObjectAccessTiming',
  ),
);

class CourseObjectAccessTiming extends CourseObjectAccess {

  public function optionsDefinition() {
    $defaults = parent::optionsDefinition();

    $defaults += array(
      'duration' => NULL,
      'release' => NULL,
      'expiration' => NULL,
      'release_hidden' => NULL,
      'expiration_hidden' => NULL,
    );

    return $defaults;
  }

  function take() {
    $time = REQUEST_TIME;

    if ($this->getOption('duration')) {
      if ($this->getCourseObject()->getFulfillment()->getOption('date_started')) {
        $duration_end = $this->getCourseObject()->getFulfillment()->getOption('date_started') + ($this->getOption('duration'));
        if ($time > $duration_end) {
          $duration_end_h = date('Y-m-d H:i:s', $duration_end);
          $this->getCourseObject()->setAccessMessage('duration-expired', t('Your enrollment in this activity expired on ') . $duration_end_h);
          return FALSE;
        }
      }
    }

    $released = $this->isReleased();

    $expired = $this->isExpired();

    return $released && !$expired;
  }

  function see() {
    if (!$this->isReleased() && $this->getOption('release_hidden')) {
      return FALSE;
    }
    if ($this->isExpired() && $this->getOption('expiration_hidden')) {
      return FALSE;
    }
  }

  function view() {
    return $this->take();
  }

  function optionsForm(&$form, &$form_state) {
    $config = $this->getOptions();

    if (module_exists('timeperiod')) {
      $form['duration'] = array(
        '#title' => t('Duration'),
        '#description' => t('Length of time a user can remain in this object before it is closed.'),
        '#type' => 'timeperiod_select',
        '#units' => array(
          '86400' => array('max' => 30, 'step size' => 1),
          '3600' => array('max' => 24, 'step size' => 1),
          '60' => array('max' => 60, 'step size' => 1),
        ),
        '#default_value' => $config['duration'],
      );
    }
    else {
      $form['duration'] = array(
        '#title' => t('Duration'),
        '#description' => t('Length of time in seconds a user can remain in this object before it is closed.'),
        '#type' => 'textfield',
        '#size' => 8,
        '#default_value' => $config['duration'],
      );
    }

    $form['release'] = array(
      '#title' => t('Release date'),
      '#description' => t('When this object can be accessed. If this object is required, users will not be able to proceed until after this date.'),
      '#type' => 'date_popup',
      '#default_value' => $config['release'],
    );

    $form['expiration'] = array(
      '#title' => t('Expiration date'),
      '#description' => t('When this object will close. If this object is required, users will not be able to proceed to the next activity after this date.'),
      '#type' => 'date_popup',
      '#default_value' => $config['expiration'],
    );

    $form['release_hidden'] = array(
      '#title' => t('Hide until release date'),
      '#type' => 'checkbox',
      '#default_value' => $config['release_hidden'],
    );

    $form['expiration_hidden'] = array(
      '#title' => t('Hide after expiration date'),
      '#type' => 'checkbox',
      '#description' => t('Hide the object after the expiration. For example, an optional pre-test that expires.'),
      '#default_value' => $config['expiration_hidden'],
    );

    return $form;
  }

  function isReleased() {
    if (REQUEST_TIME <= strtotime($this->getOption('release'))) {
      $this->getCourseObject()->setAccessMessage('not-open', t('This activity will be available on @release.', array('@release' => $this->getOption('release'))));
      return FALSE;
    }
    else {
      return TRUE;
    }
  }

  function isExpired() {
    if ($this->getOption('expiration') && REQUEST_TIME > strtotime($this->getOption('expiration'))) {
      $this->getCourseObject()->setAccessMessage('closed', t('This activity closed on @expiration.', array('@expiration' => $this->getOption('expiration'))));
      return TRUE;
    }
    else {
      return FALSE;
    }
  }

}
