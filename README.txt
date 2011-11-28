
Course module
=============

Allows any content-type to be used as an e-learning course, containing any
number of graded or ungraded course objects.

Features
--------
* Course object API (Drupal LMS)
    * Allow any Drupal node to be part of a course requirement workflow
    * Built in support for several course objects (see Getting started)
* Course API to allow access to taking courses/enrolling into courses
* External LMS support such as Moodle
    * Built-in Moodle/Drupal course integration, SSO
* Views integration, including several default views for course listings and
  user status

Dependencies
------------

* Autoload

Getting started
---------------

1. Enable course module, and a bundled course object:
    * course_quiz - Graded Quiz object
    * course_poll - Poll requirement
    * course_webform - Webform submission requirement
    * course_content - Use any content type as a course object
    * course_certificate - Award a Certificate on course completion
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

Ubercart support
----------------

* Course comes bundled with course_uc, which provides Ubercart actions to enroll
  a user after purchasing a course product.

Course credit
-------------

* Course comes bundled with course_credit, which will allow an admin to assign
  and map credit types to learner profiles and courses. Learners will then be
  able to receive or claim credit that they are eligible for on completion of a
  course. Credit can appear in a completed activities view and is exposed to
  Token for use in a module like Certificate.

Development branch
------------------

* 6.x-1.x is under heavy development and should be considered unstable. Please
  report bugs in the issue queue.

Planned
-------

* Integration with the Rules module for access to taking courses
* Supporting SCORM as a course object

Documentation
-------------

* See course.api.php

Credits
-------

* This project is sponsored by DLC Solutions
