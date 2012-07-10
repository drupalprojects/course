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
  $requirements = $node->course['objects'];
  if (is_array($form_state['values']['course_outline']['requirements'])) {
    $requirements = $form_state['values']['course_outline']['requirements'];
  }

  $cform['#title'] = 'Course objects';
  $form['#theme'] = 'course_outline_overview_form';

  // These are the available objects.
  $course_objects = course_get_handlers('object');
  $options = array(
    '' => '- select new -',
  );
  foreach ($course_objects as $module => $objects) {
    if ($objects) {
      if (count($objects) == 1) {
        $pair = each($objects);
        $key = $pair[0];
        $object = $pair[1];
        $options[$module . '-' . $key] = $object['name'];
      }
      else {
        foreach ($objects as $key => $object) {
          $module_string = ucwords(str_replace('_', ' ', $module));
          $options[$module . '-' . $key] = $module_string . ': ' . $object['name'];
        }
      }
    }
  }

  // Wrapper for just the objects.
  $cform['requirements']['#tree'] = TRUE;
  if (count($requirements)) {
    foreach ($requirements as $uniqid => $requirement) {
      $requirement = (array) $requirement;
      if (is_numeric($requirement['coid'])) {
        $courseObject = course_get_course_object($requirement);
      }
      else {
        $sess = & $_SESSION['course'][$node->nid]['editing'][$uniqid];
        $courseObject = course_get_course_object($sess['module'], $sess['object_type']);
      }
      if ($courseObject) {
        foreach ($courseObject->getWarnings() as $warning) {
          drupal_set_message(filter_xss($warning), 'warning');
        }
      }
      $rform = _course_outline_requirement_form($requirement, $uniqid, $courseObject);

      if (empty($requirement['coid'])) {
        // User hasnt selected an object type yet, let them select.
        $rform['object_type'] = array(
          '#type' => 'select',
          '#options' => $options,
          '#default_value' => $requirement['object_type'],
        );
      }
      else {
        // Don't allow user to change type of object.
        $rform['object_type'] = array(
          '#type' => 'hidden',
          '#value' => $requirement['object_type'],
        );
        $rform['object_type_show'] = array(
          '#type' => 'markup',
          '#value' => $course_objects[$requirement['module']][$requirement['object_type']]['name'] . '<br/><small><i>'
          . ucwords(str_replace('_', ' ', $requirement['module'])) . '</i></small>',
        );
      }
      ctools_ajax_associate_url_to_element($form, $rform['object_type'], "course-object/nojs/store/$uniqid");
      $cform['requirements'][$uniqid] = $rform;
    }
  }

  // Add a blank row.
  if ($form_state['values']['op'] != 'Save') {
    $obj_uniqid = uniqid();
    $new['nid'] = $node->nid;
    $new = _course_outline_requirement_form($new, $obj_uniqid);
    $new['object_type'] = array(
      '#title' => 'Component',
      '#type' => 'select',
      '#options' => $options,
    );
    ctools_ajax_associate_url_to_element($form, $new['object_type'], "course-object/nojs/store/$obj_uniqid");
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

  //$form['#redirect'] = "node/$node->nid/course-outline";

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
function _course_outline_requirement_form($requirement = array(), $uniqid = NULL, $courseObject = NULL) {
  $rform['#tree'] = TRUE;

  $rform['coid'] = array(
    '#type' => 'value',
    '#default_value' => $requirement['coid'],
  );

  $rform['instance'] = array(
    '#type' => 'value',
    '#default_value' => $requirement['instance'],
  );

  $rform['module'] = array(
    '#type' => 'value',
    '#default_value' => $requirement['module'],
  );

  $rform['object_type'] = array(
    '#type' => 'value',
    '#default_value' => $requirement['object_type'],
  );

  $rform['nid'] = array(
    '#type' => 'value',
    '#default_value' => $requirement['nid'],
  );

  if ($courseObject) {
    // Do not use prefix/suffix because markup only renders with a value, and we
    // need the wrapper before the title is saved for ajax population after each
    // settings modal update.
    $title = '<div id="title-' . $uniqid . '">';
    $title .= check_plain($courseObject->getTitle());
    $title .= '</div>';
    $rform['title'] = array(
      '#value' => $title,
    );

    $summary = '<div id="summary-' . $uniqid . '">';
    if (!empty($requirement['coid'])) {
      $summary .= filter_xss_admin($courseObject->renderOptionsSummary());
    }
    $summary .= '</div>';
  }
  $rform['summary'] = array(
    '#value' => $summary,
  );

  $rform['settings'] = array(
    '#value' => l(t('Settings'), "node/" . $requirement['nid'] . "/course-object/nojs/$uniqid/edit", array('attributes' => array('class' => "ctools-use-modal settings-$uniqid"))),
  );

  $rform['weight'] = array(
    '#type' => 'weight',
    '#default_value' => $requirement['weight'],
    '#attributes' => array(
      'class' => 'course-object-weight',
    ),
  );

  $rform['dummy'] = array(
    '#type' => 'checkbox',
    '#process' => array('ctools_dependent_process'),
    '#dependency' => array('dummy' => 1),
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
  drupal_add_css(drupal_get_path('module', 'course') . '/css/admin.css');

  $headers = array(t('Title'), t('Type'), t('Operations'), t('Weight'));

  $rows = array();

  foreach (element_children($requirements) as $key) {
    $requirement = & $requirements[$key];
    unset($requirements[$key]['title']['#title']);
    unset($requirements[$key]['object_type']['#title']);
    $rows[$key]['data'][] = drupal_render($requirements[$key]['title']) . drupal_render($requirements[$key]['summary']);
    $rows[$key]['data'][] = drupal_render($requirements[$key]['object_type']) . drupal_render($requirements[$key]['object_type_show']);
    // @todo find an elegant way to hide the settings link until a component
    //   type is selected.
    //$new = empty($requirements[$key]['coid']['#value']);
    $rows[$key]['data'][] = $new ? '' : drupal_render($requirements[$key]['settings']);

    // Add draggable settings to row.
    $rows[$key]['class'] = 'draggable';
    $rows[$key]['data'][] = drupal_render($requirements[$key]['weight']);

    // Add id to row for per-row ajax handling.
    $rows[$key]['id'] = "row-{$key}";

    // Deletion handling.
    $nid = $requirement['nid']['#value'];
    // @todo should we unify this, so we always use $courseObject->getOptions()
    // instead of pulling from the session?
    if ($_SESSION['course'][$nid]['editing'][$key]['delete']) {
      $rows[$key]['class'] = 'deleted';
    }

    // Mark everything else as printed.
    drupal_render($requirements[$key]);
  }
  $table = theme('table', $headers, $rows, array('id' => 'course-objects'));
  $table .= drupal_render($form);
  return $table;
}

/**
 * Validation callback.
 */
function _course_outline_overview_validate_requirements(&$form, &$form_state) {

}

/**
 * Submit handler.
 */
function course_outline_overview_form_submit(&$form, &$form_state) {
  $node = node_load($form['nid']['#value']);

  // Get form state values for object elements on the course outline overview:
  // - An associative array of course objects, keyed by ID. The ID for already
  //   saved objects is {course_outline}.coid, but for AHAH created objects the
  //   key is a generated unique ID until save.
  //   - coid: The key loaded from the database. If empty, the object is new.
  //   - module: The implementing module name (course_quiz etc).
  //   - object_type: The course object key as defined by
  //     hook_course_handlers().
  $objects = $form_state['values']['course_outline']['requirements'];

  // Get object handler definitions for labels.
  $object_info = course_get_handlers('object');

  foreach ($objects as $object_key => $object) {
    // We use the object_type namespace for two purposes:
    // - The form element is a select list with keys matching the combined
    //   pattern: {module}-{object_type}.
    // - We save only the second part of that combined key to the database.
    if (!$object['module']) {
      // Repurpose the requirement type array element.
      list($object['module'], $object['object_type']) = explode('-', $object['object_type']);
    }

    if (!$object['object_type']) {
      continue;
    }

    // Get each course object settings saved form values, which are not on the
    // outline overview form.
    $courseObject = course_get_course_object($object);
    $options = $courseObject->getOptions();

    // If title is not specified, set title from component.
    if (!$courseObject->getTitle() && isset($object['object_type'])) {
      // Get the component name from object info.
      $title = $object_info[$object['module']][$object['object_type']]['name'];
      $courseObject->setOption('title', $title);
    }

    // We only save overview objects with a selected component, whether loaded
    // from the database or AHAH created.
    if (isset($object['object_type'])) {
      // Delete database loaded objects that are on the chopping block.
      $is_loaded = $object['coid'];
      if ($options['delete'] && $is_loaded) {
        if ($options['delete_instance']) {
          // Also delete related object instance(s) if specified.
          $courseObject->delete();
        }
        course_outline_delete_requirement($object);
      }
      // If not slated for deletion, save the both db loaded and AHAH objects.
      elseif (!$options['delete']) {
        // Set the options, but don't overwrite any that weren't provided.
        foreach ($object as $element => $value) {
          $courseObject->setOption($element, $value);
        }

        // Hey, now we have an ID. We might have settings for objects that didn't
        // exist until now. Save them.
        // @todo do we still need this?
        if ($editing_object = $_SESSION['course'][$courseObject->getCourseNid()]['editing'][$object_key]) {
          foreach ($editing_object as $editing_key => $value) {
            if (!is_null($value)) {
              $courseObject->setOption($editing_key, $value);
            }
          }
        }

        // If there is no instance ID, create one.
        if (!$courseObject->getInstanceId()) {
          $courseObject->create();
        }

        // If we get this far, save the object.
        $courseObject->save();
      }
    }
  }
  // Discard any session elements that did not pass criteria for saving.
  unset($_SESSION['course'][$node->nid]['editing']);

  drupal_set_message('Updated course.');

  // @todo use form #redirect instead of drupal_goto()?
  drupal_goto("node/$node->nid/course-outline");
}

/**
 * Generate HTML of the course outline.
 *
 * @param object $node
 *
 * @return course outline list.
 */
function course_outline_list($node) {
  global $user;

  $course = course_get_course($node, $user);

  // Iterate over objects.
  foreach ($course->getObjects() as $key => $courseObject) {
    $step = array();

    if (!$courseObject->access('see')) {
      if ($status = $courseObject->getStatus()) {
        drupal_set_message(filter_xss($status), 'info');
      }
      continue;
    }
    if ($courseObject->access('take')) {
      // User can take this course object.
      $step['link'] = $courseObject->getUrl();

      if (!$prev_req || $prev_req->getFulfillment()->isComplete()) {
        $step['status'] = 'Begin';
      }

      // Step is complete.
      if ($courseObject->getFulfillment()->isComplete()) {
        $step['class'] = 'completed';
        $step['status'] = 'Complete';
        $step['image'] = 'misc/watchdog-ok.png';
      }
      elseif ($courseObject->getFulfillment()->getId()) {
        $step['status'] = 'Continue';
        $step['class'] = 'continue';
        $step['image'] = 'misc/menu-collapsed.png';
        if ($course->getActive() === $courseObject) {
          $step['status'] = $courseObject->getStatus();
          if (!$step['status'] && $courseObject->getCourse()->getActive()->getId() == $courseObject->getId()) {
            $step['status'] = 'In progress';
            $step['class'] = 'in-progress';
            $step['image'] = 'misc/watchdog-warning.png';
          }
        }
      }
    }
    else {
      // User cannot access this step yet.
      $step['style'] = 'not-started';
      $step['status'] = implode('<br/>', $courseObject->getAccessMessages());
    }

    $img = theme('image', $step['image']);

    if (!$step['link']) {
      $data = $courseObject->getTitle() . '<br/><span class="course-outline-status">' . $step['status'] . '</span>';
    }
    else {
      $data = l("$img {$courseObject->getTitle()}<br/>", $step['link'], array('html' => TRUE)) . '<span class="course-outline-status">' . $step['status'] . '</span>';
    }
    $workflow[] = array(
      'data' => $data,
      'id' => $step['id'],
      'class' => $step['style'],
    );

    // We have to clone it, otherwise $courseObject becomes the next object.
    $prev_req = clone $courseObject;
  }

  if ($prev_req && $prev_req->getFulfillment()->isComplete()) {
    $workflow[] = array(
      'data' => l("$img Complete", "node/$node->nid/course-complete", array('html' => TRUE)),
      'id' => 'complete',
    );
  }

  if ($workflow) {
    $output .= '<div id="course-toc">';
    $output .= '<div id="course-toc-trigger" class="trigger"></div>';
    $output .= theme('item_list', $workflow);
    $output .= '</div>';
  }
  return $output;
}

/**
 * Render a landing page for course completion.
 *
 * @param stdClass $course_node A course node.
 *
 * @return string HTML for the landing page.
 *
 * @todo change the name of this function (since it's more than just completion
 * links).
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
    // Allow modules to add links to the course completion landing page, such as
    // post-course actions.
    drupal_alter('course_outline_completion_links', $links, $course_node, $account);
    foreach ($links as $link) {
      // @todo change the way $links work. Replace the three element array with
      // something else. Maybe $element and $value from theme_form_element() if
      // we must be this controlling?
      // Example:
      // @code
      //   $element = t('A description');
      //   $value = l($text, $path);
      //   $links = array('element' => $element, 'value' => $value).
      // @endcode
      // But why not allow modules to alter the landing page however they want
      // (instead of requiring 'links' with this very specific output format)?
      $txt_out .= theme('form_element', array('#description' => $link[2]), l($link[0], $link[1]));
    }
  }
  else {
    $requirements_outstanding = course_outline_check_remaining_requirements($course_node, $account);
    $items = array();
    foreach ($requirements_outstanding as $req) {
      $status_css = $req->complete ? 'complete' : 'incomplete';
      $status_img = $req->complete ? 'ok' : ($req->required ? 'error' : 'warning');
      $status_optional = ($status_img == 'warning') ? ' (optional)' : '';
      $grade = $req->graded ? ' - Your grade: ' . $req->grade_result . '%, Pass grade: ' . $req->passing_grade . '%' : '';
      $items[] = array(
        array(
          'data' => theme('image', "misc/watchdog-{$status_img}.png", $status_css),
          'width' => 20,
        ),
        $req->title . $status_optional . $grade,
      );
    }
    $reqs_txt = theme('table', NULL, $items);

    $txt_out .= '<p>' . t('You must complete the remaining requirements to proceed.') . '</p>';
    $txt_out .= $reqs_txt . '<div class="homelink"><p>' . l($course_out_return_array['course_outline_link_text'], $course_out_return_array['course_outline_link']) . '</p></div>';

    $links = array(
      'course' => array('Return to course', "node/$course_node->nid/takecourse", 'Return to the course to view course details and material.'),
    );
    // Allow modules to alter remaining requirement links on the course
    // completion landing page.
    drupal_alter('course_outline_incomplete_links', $links, $course_node, $account);
    foreach ($links as $link) {
      $txt_out .= theme('form_element', array('#description' => $link[2]), l($link[0], $link[1]));
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
  left join {course_outline_fulfillment} crf on (cr.coid = crf.coid and uid = %d)
  where nid = %d and enabled
  order by weight asc";

  $results = db_query($sql, $account->uid, $course_node->nid);
  while ($row = db_fetch_object($results)) {
    $requirements_outstanding[$row->coid] = $row;
  }

  return $requirements_outstanding;
}

/**
 * Delete a fulfillment.
 */
function course_outline_delete_fulfillment($mixed) {
  $sql = "delete from {course_outline_fulfillment} where coid = %d and uid = %d";
  db_query($sql, $mixed->coid, $mixed->uid);
  return db_affected_rows() > 0;
}
