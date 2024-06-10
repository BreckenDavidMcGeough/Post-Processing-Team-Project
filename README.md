# POST Process Project (team-based)

## DUE

This project is due no later than the last day of classes at 11:59 PM

## Description

Write a C program that reads FDA recall data from an XML file (whose filename is specified on the command line), and then enters an interactive mode in which the commands described below are supported.  The command mode prompt must be “? ”. The data must be stored in a data structure of your choosing.

The [FDA Recalls Data Sets](https://www.fda.gov/AboutFDA/Transparency/OpenGovernment/ucm225433.htm) is also available locally on `cerf.cse.buffalo.edu` in the directory `/shared/projects/alphonce/Data/FDA`.

The XML file contains records of the following form:

```xml
<PRODUCT>
  <DATE>Fri, 21 Apr 2017 21:16:00 -0400</DATE>
  <BRAND_NAME><![CDATA[Roundy’s Brand and Harris Teeter Brand]]></BRAND_NAME>
  <PRODUCT_DESCRIPTION><![CDATA[Frozen hash browns]]></PRODUCT_DESCRIPTION>
  <REASON><![CDATA[May be contaminated with extraneous golf ball materials.]]></REASON>
  <COMPANY><![CDATA[McCain Foods USA, Inc.]]></COMPANY>
  <COMPANY_RELEASE_LINK>https://www.fda.gov/Safety/Recalls/ucm554452.htm</COMPANY_RELEASE_LINK>
  <PHOTOS_LINK>   </PHOTOS_LINK>
</PRODUCT>
```

The XML file may optionally contain some header information, which may include an entry of this form:

```xml
<SEARCH>
<BASE>RecallsDataSet2015-2017.xml</BASE>
<FILTER><BRAND_NAME>Roundy</BRAND_NAME></FILTER>
<FILTER><REASON>golf</REASON></FILTER>
</SEARCH>
```

The interpretation of this header is given below.  Any other non-record information in the file can be ignored.

Here are the commands you must support:

* `filter FILTER` - This command sets the current dataset to the set of records that satisfy the FILTER.

A basic FILTER is an ‘in’ filter, of the form <keyword> in `<field>`.  A record satisfies this filter if it contains the indicated keyword in the indicated field. For example, here are three sample commands using ‘filter’:

```
filter "golf" in REASON
filter "golf ball" in REASON
filter "2017" in DATE
```

Another basic FILTER is a ‘notin’ filter, of the form `<keyword> notin <field>`.  A record satisfies this filter if it does NOT contain the indicated keyword in the indicated field.

There are two special `<field>` specifications: ANY and ALL: 

* `<keyword> in ANY` is matched if the `<keyword>` appears in any field of the record

* `<keyword> in ALL` is matched if the `<keyword>` appears in all fields of the record

* `<keyword> notin ANY` is matched if there is a field of the record that the `<keyword>` does not appear in.

* `<keyword> notin ALL` is matched if there is no field of the record that the `<keyword>` appears in.

FILTERs can be combined with OR.  If F1 and F2 are filters, so is F1 OR F2.  A record matches F1 OR F2 if it matches F1 or if it matches F2.

```
filter "golf" in REASON
filter "golf ball" in REASON
filter "2017" in DATE OR "2018" in DATE
```

Although FILTERs cannot be combined with AND, applying two filters one after the other achieves the same effect, as in filter F1 followed by filter F2.

* `reset` – resets the current dataset to what was loaded at startup.

* `save <filename>` – saves filename of the loaded dataset, followed by the ordered list of commands that produced the current dataset from the last startup or reset command, followed by the XML of the records in the current dataset.

For example:
Suppose the dataset RecallsDataSet2015-2017.xml is loaded, and that the following commands are issued:

```
filter “Roundy” in BRAND_NAME
filter “golf” in REASON
save “oddball”
```

must save the following in a file named “oddball.xml”:

```xml
<SEARCH>
<BASE>RecallsDataSet2015-2017.xml</BASE>
<FILTER><BRAND_NAME>Roundy</BRAND_NAME></FILTER>
<FILTER><REASON>golf</REASON></FILTER>
</SEARCH>
<PRODUCT>
  <DATE>Fri, 21 Apr 2017 21:16:00 -0400</DATE>
  <BRAND_NAME><![CDATA[Roundy’s Brand and Harris Teeter Brand]]></BRAND_NAME>
  <PRODUCT_DESCRIPTION><![CDATA[Frozen hash browns]]></PRODUCT_DESCRIPTION>
  <REASON><![CDATA[May be contaminated with extraneous golf ball materials.]]></REASON>
  <COMPANY><![CDATA[McCain Foods USA, Inc.]]></COMPANY>
  <COMPANY_RELEASE_LINK>https://www.fda.gov/Safety/Recalls/ucm554452.htm</COMPANY_RELEASE_LINK>
  <PHOTOS_LINK>   </PHOTOS_LINK>
</PRODUCT>
```

The ```<SEARCH>``` part for the following

```
filter "golf" in REASON
filter "golf ball" notin REASON
filter "2017" in ANY or "2018" notin ALL
```

could be as shown below.  The spacing helps readability, but does not affect interpretation.  Opening tags in XML can have attribute-value pairs.  This shows the specification of a value for the ```type``` attribute for three of the filters.  If ```type``` is not specified it is assumed to be ```in```.

```xml
<SEARCH>
  <BASE>RecallsDataSet2015-2017.xml</BASE>
  <FILTER>
    <REASON>
      golf
    </REASON>
  </FILTER>
  <FILTER type="notin">
    <REASON>
      golf ball
    </REASON>
  </FILTER>
  <OR>
    <FILTER type="in">
      <ANY>
        2017
      </ANY>
    </FILTER>
    <FILTER type="notin">
      <ALL>
        golf
      </ALL>
    </FILTER>
  </OR>
</SEARCH>

```

If the program is started with this file as an argument it must read the RecallsDataSet2015-2017.xml file, and then perform the two filter operations.  A ‘reset’ operation must reset the current dataset to what was originally read from the base file, RecallsDataSet2015-2017.xml.

If there is already a file named `<filename>.xml` in the filesystem then the program does not save the dataset to file, but rather announces to the user that the file already exists and instructs them to use overwrite `<filename>` instead.

* `overwrite <filename>` – similar to save, expect it overwrites an existing file with the name `<filename>.xml`. If there is NOT already a file named `<filename>.xml` in the filesystem then the program does not save the dataset to file, but rather announces to the user that no such file exists and instructs them to use `save <filename>` instead.

* `count` – prints the number of records in the current dataset, but does not change the current dataset.

* `print` – prints the records in the current dataset, but does not change the current dataset.

* `help` – prints a summary of the available commands, but does not change the current dataset.

* `quit` – exits the program, but asks for confirmation to exit if the current dataset has not been saved.  If the current dataset has been save the program exits immediately.


## Nota Bene
The tags used in the XML data files changed between 2017 and 2018.  Ideally your program will work regardless of which tags are used.

## Questions/Clarifications
Post questions in Piazza.

## Assessment
You can view the rubric for the POST project in UBLearns.
