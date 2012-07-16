
Course module
=============

Create Drupal e-learning courses.

Features
--------
* Allows any content-type to be used as an e-learning course, containing any
  number of graded or ungraded course objects.
* Course object API to define learning objects to be added to a workflow
    * Built in support for Drupal nodes to be part of a course object workflow.
    * Built in support for several course objects (see Getting started)
    * Extensible to allow other content/assessments or non-Drupal (external)
      objects to be delivered and tracked
* Course API to allow access to taking courses/enrolling into courses
* Framework for external learning application integration (such as Moodle).
* Views 3 integration, including several default views for course listings and
  user status

Dependencies
------------

* Chaos tools for ahah/modal forms.
* Views 3.x (optional) for most reports and user transcript.
* AHAH Helper 2.x for some form functionality.
* The Drupal 6 version of Course requires Autoload 2.x.


Getting started
---------------

1. Enable course module, and a bundled course object:
    * course_book - Use Drupal Books as course objects
    * course_certificate - Award a Certificate on course completion
    * course_content - Use any content type as a course object
    * course_object_manual - Arbitrary steps which must be marked complete by an
      administrator before a learner may proceed past them
    * course_poll - Poll object
    * course_quiz - Graded Quiz object
    * 6.x - course_scorm - Exposes cck_scorm fields as Course objects
    * course_webform - Webform submission object
2. Set up the "Course outline" block at admin/build/blocks
3. Go to Create content -> Course
4. Add new course objects, "Quiz" will be available
5. Go to the "take course" tab, and set up questions for your quizzes.
6. Take the course!

Enrollments and attendance
--------------------------

* Course comes bundled with course_signup, to use Signup as an enrollment and
  attendance management system. Attendance can be a requirement for completion
  of a live course.

E-commerce support
------------------

* 6.x - Course comes bundled with course_uc, which provides Ubercart actions to
  enroll a user after purchasing a course product.
* 7.x - Course is planning to support Commerce.

Course credit
-------------

* Course comes bundled with course_credit, which will allow an admin to assign
  and map credit types to learner profiles and courses. Learners will then be
  able to receive or claim credit that they are eligible for on completion of a
  course. Credit can appear in a completed activities view and is exposed to
  Token for use in a module like Certificate.
* Course also includes course_restrict_credit, which restricts claiming credit
  to only one of many similar courses.

Reporting
---------

* Course report areas for global (course-level) reports and individual
  (object-level) reports.
* API to allow course objects to provide their own reports.


Development branches
--------------------

* 6.x-1.x is under heavy development and should be considered unstable. Please
  report bugs in the issue queue.
* 7.x-1.x - Only use this branch if you want to help with #1116740: Port to
  Drupal 7 (Course)

Release schedule
----------------

* #1517330: 6.x-1.0 release
* #1116740: Port to Drupal 7 (Course)

Roadmap
-------

* #1649996: Course Services integration
* #1390058: Support for SCORM
* #1361754: Support books as course objects
* #1650646: LTI support for external courses and objects

Documentation
-------------

* See course.api.php

Credits
-------

* This project is sponsored by DLC Solutions for EthosCE
