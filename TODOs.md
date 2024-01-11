TODOs
=====
- Allow the specification of escaped characters in IpFormMenu (e.g \n,\r,etc.)
- Include testing.
- Move MyCSV/MyCol into their own repository.
- Write documentation for MyCSV/MyCol.
- Implement a list form of MyCol(maybe).
- Improve the usability of the table preview button.
   - Currently only so much can be done about the performance, as it's
     largely limited by FTXUI.
   - At the very least, I would like to implement the ability to make larger
     structural changes to the given table. For example, the ability to move
     columns around to different positions.
- Further cleaning mechanisms. Some ideas would be:
   - The ability to remove all empty rows.
   - Deletion based on some form of search algorithm(i.e fuzzy and/or regex)
     and/or with the `Unique Words` list.
- Implement various search/sort algorithms
- Clean up certain portions of the code.
- Convert MyCol into a template to aid in processing numeric data types(when/if
  statistical routines are implemented here).
