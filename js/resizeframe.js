function resizeFrame(obj) {
  var frame = jQuery(obj);
  frame.contents().find('body').bind('DOMAttrModified DOMSubtreeModified DOMNodeInserted DOMNodeInsertedIntoDocument', function () {
    frame.height(frame.contents().find("body").height());
  });


}
