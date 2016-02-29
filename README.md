mammon
======
>Of greed

A remote resource archiving framework.

This is a framework for writing crawlers that archive targeted content from websites. Content is saved in a database for later analysis and utilisation. My personal use case for this framework is for documenting and analysing conversations between internet users.

At the moment only chan style image boards are supported since they are all based on more or less that same source code and will provide the greatest coverage of data with minimal amount of code. Later I plan to add support for more traditional text based forums such as phpBB.

This framework is written in C++ and uses:

* Boost Boost.PropertyTree for parsing JSON
* Ruby's Nokogiri for parsing HTML (integrated into C++)
* SQLite for database storage
* Scripting support with Chibi Scheme for rapid prototyping of selection and filtering algorithms
