<?php

/**
 * @file course_outline.inc
 *
 * Functions to show and edit graphical course outline.
 */

/**
 * Theme all requirements as a table.
 */
function course_outline_overview_form(&$form_state) {
  $form = array();

  // Register the form with ahah_helper so we can use it. Also updates
  // $form_state['storage'] to ensure it contains the latest values that have
  // been entered, even when the form item has temporarily been removed from
  // the form. So if a form item *once* had a value, you *always* can retrieve
  // it.
  ahah_helper_register($form, $form_state);

  // Determine the default value of the 'usage' select. When nothing is stored
  // in $form_state['storage'] yet, it's the form hasn't been submitted yet,
  // thus it's the first time the form is being displayed.
  // Load the modal library and add the modal javascript.
  ctools_include('ajax');
  ctools_include('modal');
  ctools_include('dependent');
  ctools_include('object-cache');
  ctools_modal_add_js();

  // Wrapper for objects and more button.
  $form['#tree'] = TRUE;
  $form['#prefix'] = '<div class="clear-block" id="course-outline-wrapper">';
  $form['#suffix'] = '</div>';

  // Shortcut.
  $cform = &$form['course_outline'];

  if ($form_state['values']['nid']) {
    $node = node_load($form_state['values']['nid']);
  }
  else {
    $node = node_load(arg(1));
  }

  $form['nid']['#type'] = 'hidden';
  $form['nid']['#value'] = $node->nid;

  // Grab requirements.
  $requirements = $node->course_outline['requirements'];
  if (is_array($form_state['values']['course_outline']['requirements'])) {
    $requirements = $form_state['values']['course_outline']['requirements'];
  }

  $cform['#title'] = 'Course objects';
  $form['#theme'] = 'course_outline_overview_form';

  // These are the available objects.
  $course_objects = course_available_objects();
  $options = array(
    '' => '- select new -',
  );
  foreach ($course_objects as $module => $objects) {
    if (count($objects) == 1) {
      $pair = each($objects);
      $key = $pair[0];
      $object = $pair[1];
      $options[$object['module'] . '-' . $key] = $object['title'];
    }
    else {
      foreach ($objects as $key => $object) {
        $module_string = ucwords(str_replace('_', ' ', $module));
        $options[$object['module'] . '-' . $key] = $module_string . ': ' . $object['title'];
      }
    }
  }

  // Wrapper for just the objects.
  $cform['requirements']['#tree'] = TRUE;
  if (count($requirements)) {
    foreach ($requirements as $uniqid => $requirement) {
      $requirement = (array) $requirement;
      if ($requirement['hidden']) {
        // Do not print out hidden requirements yet, such as attendance/payment.
        continue;
      }
      $rform = _course_outline_requirement_form($requirement, $uniqid);
      if (empty($requirement['snid'])) {
        // User hasnt selected an object type yet, let them select.
        $rform['requirement_component'] = array(
          '#type' => 'select',
          '#options' => $options,
          '#default_value' => $requirement['requirement_component'],
        );
      }
      else {
        // Don't allow user to change type of object.
        $rform['requirement_component'] = array(
          '#type' => 'hidden',
          '#value' => $requirement['requirement_component'],
        );
        $rform['requirement_component_show'] = array(
          '#type' => 'markup',
          '#value' => ucwords(str_replace('_', ' ', $requirement['requirement_type'])) . '<br/>' . $course_objects[$requirement['requirement_type']][$requirement['requirement_component']]['title'],
        );
      }
      ctools_ajax_associate_url_to_element($form, $rform['requirement_component'], "course-object/nojs/store/$uniqid");
      $cform['requirements'][$uniqid] = $rform;
    }
  }

  // Add a blank row.
  if ($form_state['values']['op'] != 'Save') {
    $obj_uniqid = uniqid();
    $new['nid'] = $node->nid;
    $new = _course_outline_requirement_form($new, $obj_uniqid);
    $new['requirement_component'] = array(
      '#title' => 'Component',
      '#type' => 'select',
      '#options' => $options,
    );
    ctools_ajax_associate_url_to_element($form, $new['requirement_component'], "course-object/nojs/store/$obj_uniqid");
    $cform['requirements'][$obj_uniqid] = & $new;
  }

  $form['course_object_more'] = array(
    '#type' => 'submit',
    '#value' => 'Add another',
    '#ahah' => array(
      'event' => 'click',
      'method' => 'replace',
      'path' => ahah_helper_path(AHAH_HELPER_PARENTS_ENTIRE_FORM),
      'wrapper' => 'course-outline-wrapper',
    ),
    '#submit' => array('ahah_helper_submit'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Save',
    '#submit' => array('course_outline_overview_form_submit'),
  );

  $cform['requirements']['#element_validate'] = array('_course_outline_overview_validate_requirements');
  //return for form on tab
  return $form;
}

/**
 * Define a form for 1 requirement.
 *
 * To be re-used in listing requirements and creating new ones.
 */
function _course_outline_requirement_form($requirement = array(), $uniqid = NULL) {
  $rform['#tree'] = TRUE;
  $rform['snid'] = array(
    '#type' => 'value',
    '#default_value' => $requirement['snid'],
  );
  $rform['instance'] = array(
    '#type' => 'value',
    '#default_value' => $requirement['instance'],
  );
  $rform['requirement_type'] = array(
    '#type' => 'value',
    '#default_value' => $requirement['requirement_type'],
  );
  $rform['requirement_component'] = array(
    '#type' => 'value',
    '#default_value' => $requirement['requirement_component'],
  );
  $rform['nid'] = array(
    '#type' => 'value',
    '#default_value' => $requirement['nid'],
  );
  $rform['title'] = array(
    '#title' => 'Title',
    '#type' => 'textfield',
    '#size' => 16,
    '#default_value' => $requirement['title'],
    '#description' => $requirement['add'],
  );
  $rform['options']['required'] = array(
    '#title' => 'Required',
    '#type' => 'checkbox',
    '#default_value' => isset($requirement['required']) ? $requirement['required'] : 1,
  );
  $rform['options']['enabled'] = array(
    '#title' => 'Enabled',
    '#type' => 'checkbox',
    '#default_value' => isset($requirement['enabled']) ? $requirement['enabled'] : 1,
  );
  $rform['options']['delete'] = array(
    '#title' => 'Remove',
    '#type' => 'checkbox',
  );
  if ($requirement['instance']) {
    $rform['options']['delete_instance'] = array(
      '#title' => 'Delete instance',
      '#type' => 'checkbox',
      '#process' => array('ctools_dependent_process'),
      '#dependency' => array('edit-course-outline-requirements-' . $uniqid . '-options-delete' => array(1)),
    );
  }
  $rform['operations'] = array(
    '#type' => 'markup',
    '#value' => implode(' ', array(
      l('Edit', "node/" . $requirement['nid'] . "/course-object/nojs/$uniqid/edit", array('attributes' => array('class' => "ctools-use-modal settings-$uniqid"))),
    )),
  );
  $rform['weight'] = array(
    '#type' => 'weight',
    '#default_value' => $requirement['weight'],
    '#attributes' => array(
      'class' => 'course-object-weight',
    ),
  );

  return $rform;
}

/**
 * Non-ahah version for when the node is available.
 */
function course_outline_overview_add($form, $form_state) {
  // Non-ahah version.
}

function theme_course_outline_overview_form($form) {
  $requirements = & $form['course_outline']['requirements'];
  drupal_add_tabledrag('course-objects', 'order', 'sibling', 'course-object-weight');

  $headers = array('Title', 'Type', 'Options', 'Operations', 'Weight');

  $rows = array();

  foreach (element_children($requirements) as $key) {
    $requirement = & $requirements[$key];
    unset($requirements[$key]['title']['#title']);
    unset($requirements[$key]['requirement_component']['#title']);
    $rows[$key]['data'][] = drupal_render($requirements[$key]['title']);
    $rows[$key]['data'][] = drupal_render($requirements[$key]['requirement_component']) . drupal_render($requirements[$key]['requirement_component_show']);
    $rows[$key]['data'][] = drupal_render($requirements[$key]['options']);
    $rows[$key]['data'][] = drupal_render($requirements[$key]['operations']);

    // Add draggable settings to row.
    $rows[$key]['class'] = 'draggable';
    $rows[$key]['data'][] = drupal_render($requirements[$key]['weight']);

    // Mark everything else as printed.
    drupal_render($requirements[$key]);
  }
  $table = theme_table($headers, $rows, array('id' => 'course-objects'));
  $table .= drupal_render($form);
  return $table;
}

/**
 * Validation callback.
 */
function _course_outline_overview_validate_requirements(&$form, &$form_state) {
  foreach ($form_state['values']['course_outline']['requirements'] as $key => $item) {
    if ($item['requirement_component'] && !$item['title']) {
      form_error($form[$key]['title'], t('Title is required.'));
    }
  }
}

/**
 * Submit handler.
 */
function course_outline_overview_form_submit(&$form, &$form_state) {
  $node = node_load($form['nid']['#value']);
  foreach ($form_state['values']['course_outline']['requirements'] as $key => $record) {
    $record = array_merge($record, $record['options']);
    $record = (object) $record;
    $record->nid = $node->nid;
    if ($record->delete && $record->snid) {
      $courseObject = course_get_course_object($record);
      if ($record->delete_instance) {
        // Delete the instance (node, whatever).
        $courseObject->delete();
      }
      course_outline_delete_requirement($record);
    }
    else if (!$record->delete && !empty($record->requirement_component)) {
      // Must have a component.
      $courseObject = course_outline_save_requirement($record, $node);
    }

    // Hey, now we have an ID. We might have settings for objects that didn't
    // exist until now. Save them.
    if ($_SESSION['course'][$courseObject->getCourseNid()]['editing'][$key]) {
      foreach ($_SESSION['course'][$courseObject->getCourseNid()]['editing'][$key] as $key => $value) {
        $courseObject->setOption($key, $value);
      }
      $courseObject->save();
    }

    if (!$courseObject->getInstanceId()) {
      $courseObject->create();
      $courseObject->save();
    }
  }
  drupal_set_message('Updated course.');
  drupal_goto("node/$node->nid/course-outline");
}

/**
 * Generate HTML of the course outline.
 *
 * @param object $node
 *
 * @return course outline list.
 */
function course_get_outline($node) {
  global $user;

  // Array of the available course objects (to lookup modules).
  $object_map = course_available_objects();

  // Create an array of the list of course objects.
  $sql = "SELECT * FROM {course_outline} cr WHERE nid = %d AND enabled AND !hidden ORDER BY weight ASC";
  $result = db_query($sql, $node->nid);
  $course_reqs = array();
  $course_objects = array();
  while ($row = db_fetch_object($result)) {
    $course_objects[] = course_get_course_object($row, $user);
  }

  // Iterate over objects.
  foreach ($course_objects as $key => $courseObject) {
    $step = array();

    // Find out what this step's status is.
    // Check the previous step.
    $prev_req = $course_objects[($key - 1)];
    if ($courseObject->getFulfillment()->isComplete()) {
      // Step is complete. User can access.
      $step['link_style'] = 'completed';
      $step['status_text'] = 'Complete';
      $step['link_to_step'] = 'node/' . $courseObject->getCourseNid() . '/course-object/' . $courseObject->getId();
    }
    else if (!$courseObject->getFulfillment()->isComplete() && (!$prev_req || $prev_req->getFulfillment()->isComplete())) {
      // Check if this is incomplete, but next. User can access.
      $step['link_style'] = 'in-progress';
      $step['status_text'] = 'Begin';
      $step['link_to_step'] = 'node/' . $courseObject->getCourseNid() . '/course-object/' . $courseObject->getId();
    }
    else {
      // User cannot access this step yet.
      $step['link_style'] = 'not-started';
      $step['link_to_step'] = '';
    }

    // arrowitem = "<span class='arrow-step'></span>";
    $class_more = '';
    drupal_alter('course_outline_step', $step, $node, $user);
    if (!$step['link_to_step']) {
      $data = $courseObject->getTitle();
    }
    else {
      $data = l("{$courseObject->getTitle()}<br/>", $step['link_to_step'], array('html' => TRUE)) . '<span class="course-outline-status">' . $step['status_text'] . '</span>';
    }
    $workflow[] = array(
      'data' => $data,
      'id' => $step['id_text'],
      'class' => "{$step['link_style']} $class_more $arrowclass",
    );
  }

  $last_step_style = $link_style;
  $link_to_step = url("node/{$node->nid}/course-complete");

  // Allow other modules to alter the workflow if needed.
  drupal_alter('course_outline', $workflow);

  $output .= theme_item_list($workflow);

  return $output;
}

/**
 * Render a landing page for course completion.
 *
 * Modules can use hook_course_outline_completion_links_alter() to add links
 * to this page, such as post-course actions.
 *
 * @param $course_node A course node.
 *
 * @return string HTML for the landing page.
 */
function course_outline_show_complete_links($course_node) {
  global $user;
  $account = $user;
  $report = course_report_load($course_node, $account);
  if ($report->complete) {
    $txt_out .= '<p>' . t('Thank you for participating in this activity.') . '</p>';
    $links = array(
      'course' => array('Return to course', "node/$course_node->nid", 'Return to the course to view course details and material.'),
    );
    drupal_alter('course_outline_completion_links', $links, $course_node, $account);
    foreach ($links as $link) {
      $txt_out .= theme_form_element(array('#description' => $link[2]), l($link[0], $link[1]));
    }
  }
  else {
    $requirements_outstanding = course_outline_check_remaining_requirements($course_node, $account);
    $items = array();
    foreach ($requirements_outstanding as $req) {
      $status_css = $req['complete'] ? 'complete' : 'incomplete';
      $status_img = $req['complete'] ? 'ok' : ($req['required'] ? 'error' : 'warning');
      $status_optional = ($status_img == 'warning') ? ' (optional)' : '';
      $grade = $req['graded'] ? ' - Your grade: ' . $req['grade_result'] . '%, Pass grade: ' . $req['passing_grade'] . '%' : '';
      $items[] = array(
        array(
          'data' => theme_image("misc/watchdog-{$status_img}.png", $status_css),
          'width' => 20,
        ),
        $req['title'] . $status_optional . $grade,
      );
    }
    $reqs_txt = theme_table(NULL, $items);

    $txt_out .= '<p>' . t('You must complete the remaining requirements to proceed.') . '</p>';
    $txt_out .= $reqs_txt . '<div class="homelink"><p>' . l($course_out_return_array['course_outline_link_text'], $course_out_return_array['course_outline_link']) . '</p></div>';

    $links = array(
      'course' => array('Return to course', "node/$course_node->nid/takecourse", 'Return to the course to view course details and material.'),
    );
    drupal_alter('course_outline_incomplete_links', $links, $course_node, $account);
    foreach ($links as $link) {
      $txt_out .= theme_form_element(array('#description' => $link[2]), l($link[0], $link[1]));
    }
  }

  return $txt_out;
}

/**
 * Get a list of requirements and the user's fullfillment.
 */
function course_outline_check_remaining_requirements($course_node, $account) {
  $requirements_outstanding = array();

  $sql = "select crf.*, cr.* from {course_outline} cr
  left join {course_outline_fulfillment} crf on (cr.snid = crf.snid and uid = %d)
  where nid = %d and enabled
  order by weight asc";

  $results = db_query($sql, $account->uid, $course_node->nid);
  while ($row = db_fetch_object($results)) {
    $requirements_outstanding[] = $row;
  }

  return $requirements_outstanding;
}

/**
 * Delete a fulfillment.
 */
function course_outline_delete_fulfillment($mixed) {
  $sql = "delete from {course_outline_fulfillment} where snid = %d and uid = %d";
  db_query($sql, $mixed->snid, $mixed->uid);
  return db_affected_rows() > 0;
}
