Using
=====

CSV Menu
--------
   - Supply the given `Input File`, field `Delimiter`, and `Quote`. Then press `Open File`
   - The input file format is as follows:
      - A unique identifier (e.g PID) column (optional)
      - One or more response columns belonging to the associated respondents
      - Row delimiters are assumed to be either '\n'(Unix format) or '\n\r'(Windows' format)
      - Currently, column headers are assumed to be present. So if this is an issue for you, the easiest thing would be 
        to insert a blank row at the top of your table.
   - The associated data is updated by `IpForm Menu` and the `Make corrections` button of 
     `Serial Positioning & Corrections`. Said data may be saved by supplying a `Output File` and 
     pressing `Save File`. However, this is not done automatically.
      - You may see how the original file is manipulated by pressing the `Preview Table` button. Currently newlines are not rendered correctly.

IpForm Menu
-----------
   - `Black Space` & `Removles` may be given via individual characters or the `Ranges` check boxes
   - `Black Space`:
      - Definition:
         - What you want to keep as a single-sub item. Everything that is not specified by `Black Space`
           or `Removles` is assumed to be a boundary (i.e white space) between two sub-items.
      - Example:
         - If `Black Space` is set to "a-z":
            - "this,that" will become "this" & "that"
         - If `Black Space` is *also* set to ',':
            - "this,that" will stay as "this,that"
   - `Removles`:
      - Definition:
         - What you want to remove from a sub item.
      - Example:
         - If `Removles` is set to "'" and `Black Space` is set to "a-z":
            - "it's an. example" becomes "its", "an", & "example"
   - `Sub-Delim`:
      - The choice made does not affect latter processing, as such, you are free to choose whatever you like.
        So long as it is not part of `Black Space`/`Removles` (it may be fine, but I have not tested doing so).
   - `Column`:
      - The column to be cleaned/formatted
      - More than one column may be selected at a time.
   - `Clean`:
      - Formats the given column based on the supplied choices.
   - `Split`:
      - Splits the selected column(s) according to the respective sub-delimiter.
      - Currently I have only tested this split method after standardizing a column. As such, it can not (strictly speaking) be ran
        before the `Clean` action. However, a work around would be to simply apply the `Clean` action after having only supplied a sub-delim.
      - The resultant columns' headers are that of the original column + an incremented counter (origin 1).
      - Multiple columns may be selected, but have in no way been tested.
   - `Join`:
      - The opposite of `Split`. If no sub-delim is set, a space is used by default. Otherwise, it uses the 
        sub-delimiter of the left-most column.
      - The header of the left-most column is used by default.

Serial Positioning & Corrections
--------------------------------
   - `Files`:
      - `TargetDisplay`:
         - Displays an alphabetically sorted list of target words at the top of the menu. 
         - Please note, serial positioning is performed relative to the list as read from the `Target File`
         - May be toggled on/off.
      - `Target File`:
         - The name of a file containing a list of target words, with one 'target' per line
      - `Output File`:
         - The name of the output file for `SaveWords` & `Serialize & Save`. 
         - The supplied file name is cleared upon invocation of the aforementioned actions.
      - `Response Column`:
         - The index of the column(s) to parse for `Unique Words`
         - If multiple columns are selected, they will be joined into a single column according to the rules
           specified by the `Join` action present in the IpForm menu.
      - `PID Column`:
         - The index of the unique identifiers.
         - Only a single column may be selected.
         - If a column is selected as both a PID column and a response column, the PID selection takes precedence.
      - `Get Words`:
         - Parses the individual responses in `Response Column` based on the `Sub-Delim` set in `IpForm Menu`
      - `Unique Words`: 
         - Displays an alphabetically sorted list of the unique words (minus the target words) present 
           in the specified `Response Column`. You may then search through this list and enter in any corrections as you see fit.
      - `Make Corrections`:
         - Updates the individual responses according the corrections supplied in the `Unique Words` list.
         - PLEASE NOTE: If a response field contains *both* the 'original' & 'corrected' form of a sub-item,
           then the correction is not made and both forms are kept *as is*.
         - After making corrections, you may save the corrected responses via the `Output File` & `Save File`
           found in `CSV Menu`
         - Updates the list of unique words found in the table.
      - `Serialize & Save`:
         - Serializes the the sub-items of the individual responses, counts the number of intrusions, 
           and saves said data to the file supplied in the `Save File` field.
         - Saves a log file to `Serial.log` containing:
            - The original-corrected word pairs (i.e the correction you entered in the `Unique Words` list).
            - The rows corrected, and the given correction made.
