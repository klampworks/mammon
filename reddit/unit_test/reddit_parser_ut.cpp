#define BOOST_TEST_MODULE example
#include <boost/test/included/unit_test.hpp>
#include "../reddit_parser.hpp"
#include <fstream>
#include <string>
#include <iostream>

std::string dump_file(const char *filename) {

	std::ifstream ifs;
	ifs.open(filename);

	BOOST_REQUIRE_MESSAGE(ifs.good(), "Could not open file " << filename);

	std::string xml, tmp;

	while(std::getline(ifs, tmp))
		xml += tmp;

	ifs.close();
	return std::move(xml);
}

BOOST_AUTO_TEST_CASE(a)
{
	std::string xml = dump_file("reddit_list");

	reddit_parser p;
	auto ret =p.parse_posts(xml);

	std::vector<std::string> expected({
"/r/foxes/comments/21g25a/rfoxes_has_launched_a_donation_drive_for_the/",
"/r/foxes/comments/21g25a/rfoxes_has_launched_a_donation_drive_for_the/",
"http://imgur.com/gallery/AeTFk",
"http://imgur.com/gallery/AeTFk",
"http://i.imgur.com/9chXMDF.jpg",
"http://i.imgur.com/9chXMDF.jpg",
"http://i.imgur.com/xn4zN1o.jpg",
"http://i.imgur.com/xn4zN1o.jpg",
"https://www.youtube.com/watch?v=Rh39ZW-9nsI",
"https://www.youtube.com/watch?v=Rh39ZW-9nsI",
"https://www.flickr.com/photos/59777454@N02/5465823375",
"https://www.flickr.com/photos/59777454@N02/5465823375",
"http://i.imgur.com/4MusuVn.jpg",
"http://i.imgur.com/4MusuVn.jpg",
"http://i.imgur.com/WKODOkn.jpg",
"http://i.imgur.com/WKODOkn.jpg",
"http://imgur.com/a/Ggxut",
"http://imgur.com/a/Ggxut",
"http://i.imgur.com/j7EuzaJ.jpg",
"http://i.imgur.com/j7EuzaJ.jpg",
"http://i.imgur.com/ShhozE0.gif",
"http://i.imgur.com/ShhozE0.gif",
"http://i.imgur.com/UAKVH02.jpg",
"http://i.imgur.com/UAKVH02.jpg",
"http://i.imgur.com/x0fEu4j.jpg",
"http://i.imgur.com/x0fEu4j.jpg",
"https://www.flickr.com/photos/charleswelch/3559026619/",
"https://www.flickr.com/photos/charleswelch/3559026619/",
"https://www.facebook.com/230242387118779/photos/pcb.376048755871474/376048665871483/?type=1&amp;theater",
"https://www.facebook.com/230242387118779/photos/pcb.376048755871474/376048665871483/?type=1&amp;theater",
"http://bostinno.streetwise.co/2014/04/03/boston-gray-fox-spotted-on-commonwealth-avenue-pictures/",
"http://bostinno.streetwise.co/2014/04/03/boston-gray-fox-spotted-on-commonwealth-avenue-pictures/",
"https://www.facebook.com/photo.php?v=10200729960548754&amp;set=vb.136870256327166&amp;type=3&amp;theater",
"https://www.facebook.com/photo.php?v=10200729960548754&amp;set=vb.136870256327166&amp;type=3&amp;theater",
"http://i.imgur.com/lZ4HDVC.jpg",
"http://i.imgur.com/lZ4HDVC.jpg",
"http://imgur.com/C30iJH7",
"http://imgur.com/C30iJH7",
"http://imgur.com/a/CZQEG",
"http://imgur.com/a/CZQEG",
"http://ppcdn.500px.org/65863805/43c02a3c8e520e6a5c400028949f8904bfd9db49/2048.jpg",
"http://ppcdn.500px.org/65863805/43c02a3c8e520e6a5c400028949f8904bfd9db49/2048.jpg",
"http://youtu.be/c8xJtH6UcQY",
"http://youtu.be/c8xJtH6UcQY",
"http://i.imgur.com/pYsPaT2.jpg",
"http://i.imgur.com/pYsPaT2.jpg",
"http://i.imgur.com/sJFjG5R.jpg",
"http://i.imgur.com/sJFjG5R.jpg",
"https://www.facebook.com/photo.php?v=10152040421682919&amp;set=vb.38887297918&amp;type=2&amp;theater",
"https://www.facebook.com/photo.php?v=10152040421682919&amp;set=vb.38887297918&amp;type=2&amp;theater",
"http://imgur.com/yG4A5aB",
"http://imgur.com/yG4A5aB",
	});

	BOOST_CHECK(ret == expected);
}
