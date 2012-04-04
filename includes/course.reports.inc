<?php

/**
 * @file course.reports.inc
 * Reporting framework for course objects.
 */

/**
 * Page handler for course object reports.
 */
function course_object_reports_page($node) {
  $snid = arg(4);
  $report_key = arg(5);
  $report_subkey = arg(6);

  $reqs = course_outline_get_course_requirements($node);
  foreach ($reqs as $req) {
    $courseObject = course_get_course_object($req);
    $lis = array();
    if (method_exists($courseObject, 'getReports')) {
      foreach ($courseObject->getReports() as $key => $report) {
        $report_li = array(
          'data' => course_object_single_report_link($courseObject, $key, $report),
        );
        if (count($report['children'])) {
          foreach ($report['children'] as $subkey => $subreport) {
            $report_key_subkey = "$key/$subkey";
            $report_li['children'] = array(
              'data' => course_object_single_report_link($courseObject, $report_key_subkey, $subreport),
            );
          }
        }
        $lis[] = $report_li;
      }
    }
    if ($lis) {
      $nav .= theme_item_list($lis, $courseObject->getTitle());
    }
  }

  if ($snid) {
    $courseObject = course_get_course_object($reqs[$snid]);
    $report = $courseObject->getReport($report_key, $report_subkey);
    if ($report) {
      if ($report['url']) {
        $content = course_iframe($report['url']);
      }
      if ($report['content']) {
        $content = $report['content'];
      }
      $header = '<p>' . 'Viewing: ' . $courseObject->getTitle() . ' - ' . $report['title'] . '</p>';
    }
    else {
      $content = 'Sorry, no report is available for this type.';
    }
  }


  return theme('course_report', $nav, $header, $content);
}

function course_object_single_report_link($courseObject, $key, $report) {
  return l($report['title'], "node/{$courseObject->getCourseNid()}/course-reports/objects/{$courseObject->getId()}/$key");
}

function theme_course_report($nav, $header, $body) {
  $out = '';

  if (!$nav) {
    $body = t('None of the objects in your course have reports associated with them.');
  }

  $body = $header . $body;

  $row = array(
    array(
      'data' => $nav,
      'width' => 200,
      'id' => 'course-object-reports-nav',
    ),
    array(
      'data' => $body,
      'id' => 'course-object-reports-content',
    ),
  );

  $rows[] = $row;

  $out = theme_table(NULL, $rows, array(
    'id' => 'course-object-reports',
    ));

  return $out;
}
