
How to use UniTool
-----------------
The usage of the tool is:
UniTool.py [-b] [-u] [-l] [-x] [-h] [-d 'rootDirectory1'] [-d 'rootDirectory2'] [-d 'rootDirectory3']... [-q e|w]
           'rootDirectory0' 'uqiFile'|'uqiFileDirectory' ['excludedDirectory1'] ['excludedDirectory2'] ['excludedDirectory3']...

Function will sync up UQI definitions with uni files based on vfi/vfr/hfr/sd/sdi in the tree.

Required Arguments:
  'rootdirectory0'       path to root directory
  'uqiFileDirectory'     path to UQI file(UqiList.uni)
  'uqiFile'              UQI file

Options:
  -h                    Show this help
  -b                    Build option returns error if any new UQI needs assigning
                        based on vfi/vfr/hfr/sd/sdi when no -u option is specified
  -u                    Create new UQIs that does not already exist in uqiFile for
                        any string requiring a UQI based on vfi/vfr/hfr/sd/sdi
                        NOTE: 'uqiFile' cannot be readonly!
  -l                    Language deletion option (keeps only English and uqi)
                        moves all UQIs to 'uqiFile'
                        NOTE: Uni files cannot be readonly!
  -x                    Exclude 'rootDirectory'/'excludedDirectory1' &
                        'rootDirectory'/'excludedDirectory2'... from UQI list build
                        NOTE: Cannot be the same as rootDirectory
  -d                    Add multiple root directories to process
  -q                    Print warning(w) or return error(e) if different HII questions
                        are referring same string token

Return error if any duplicated UQI string or value in UQI list or if no definition
for any string referred by HII question when -b or -u is specified

NOTE: Options must be specified before parameters

Notice
-----------------
- "S0000" style will be used if uqiFile needs to be new created.
  Use the same uqi style if uqiFile is existed. For example,
  if the exist UqiFile use "\x0000" style, "\x0000" will be used.
