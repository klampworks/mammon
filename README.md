mammon
======

A remote resource archiving framework.

This is a framework for writing crawlers that will archive targeted content from websites. Content is saved in a database for later analysis and utilisation. My personal use case for this framework is for documenting and analysing conversations between internet users.

Initially only chan style image boards will be supported since they are all based on more or less that same source code and will provide the greatest coverage of data with minimal amount of code. Later I will add support for more traditional text based forums such as phpBB.

This framework is written in C++ and uses:

* the pugixml library for parsing webpages. This is a fast, non validating dom parser which makes it ideal for processing html
* SQLite for database storage.
* My own kyukon library for asynchronous networking.
