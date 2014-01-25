#define BOOST_TEST_MODULE example
#include <boost/test/included/unit_test.hpp>
#include "../chan_parser.hpp"
#include <fstream>
#include <string>

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

BOOST_AUTO_TEST_CASE(check_for_final_page) {

	chan_parser p;

	const char *first_page = "first_page.html";
	const char *final_page = "final_page.html";
	std::string xml;

	xml = dump_file(first_page);
	BOOST_CHECK(!p.final_page(xml));

	xml = dump_file(final_page);
	BOOST_CHECK(p.final_page(xml));
}

BOOST_AUTO_TEST_CASE(check_post_id) {

	chan_parser p;

	const char *xml("<a name=\"2374\"></a>");
	pugi::xml_document doc;
	doc.load(xml);

	BOOST_CHECK(p.parse_postid(doc) == "2374");
}

BOOST_AUTO_TEST_CASE(check_text) {

	chan_parser p;

	const char *xml("<blockquote>\n"
			"<p><strong>NOTHING</strong></p><p><strong>NOTHING AT ALL</strong></p>\n"
			"</blockquote>");

	const char *expected = "NOTHING NOTHING AT ALL";

	pugi::xml_document doc;
	doc.load(xml);

	BOOST_CHECK(p.parse_post_text(doc) == expected);
}

BOOST_AUTO_TEST_CASE(check_img) {

	chan_parser p;

	const char *xml("<span class=\"filesize\">File: <a target=\"_blank\""
	" href=\"http://desuchan.net/tech/src/1356867546959.jpg\">1356867546959.jpg</a>"
	" -(<em>165.0 KiB, 636x603</em>)</span>");

	const char *expected = "http://desuchan.net/tech/src/1356867546959.jpg";

	pugi::xml_document doc;
	doc.load(xml);

	BOOST_CHECK(p.parse_post_img(doc) == expected);
}

BOOST_AUTO_TEST_CASE(check_img_name) {

	chan_parser p;

	const char *xml("<span class=\"filesize\">File: <a target=\"_blank\""
	" href=\"http://desuchan.net/tech/src/1356867546959.jpg\">1356867546959.jpg</a>"
	" -(<em>165.0 KiB, 636x603</em>)</span>");

	const char *expected = "1356867546959.jpg";

	pugi::xml_document doc;
	doc.load(xml);

	BOOST_CHECK(p.parse_post_img_name(doc) == expected);
}


BOOST_AUTO_TEST_CASE(check_parse_a_thread) {

	chan_parser p;

std::vector<chan_post> expected({
{"test", "2374", "2374", "1356806391743.png", "http://desuchan.net/tech/src/1356806391743.png",
"What are you working on?"},
{"test", "2374", "2375", "1356867546959.jpg", "http://desuchan.net/tech/src/1356867546959.jpg", "NOTHING NOTHING AT ALL"},
{"test", "2374", "2378", "", "", "A java fizzbuzz program"},
{"test", "2374", "2400", "1369856779897.jpg", "http://desuchan.net/tech/src/1369856779897.jpg", ">>2378 Tryin` to make some shit in Python. Now it`s MAL parser. http://rghost.net/46363268"},
{"test", "2374", "2404", "1372814015705.jpg", "http://desuchan.net/tech/src/1372814015705.jpg", ">>2374 nothing"},
{"test", "2374", "2411", "1376735987000.jpg", "http://desuchan.net/tech/src/1376735987000.jpg", "figuring out how to make access with vba scripting take an exported excel file, convert it to a drop down table, and email it, just at the press of a button... because the people i work with are too stupid to attach a file and send an email themselves Last edited 13/08/17(Sat)06:39."},
});

	std::string xml = dump_file("thread_page.html");

	std::vector<chan_post> ret = p.parse_thread("test", xml);
	
	std::cout << ret.size() << std::endl;

	for (unsigned i = 0; i < ret.size(); i++) {
		BOOST_CHECK(ret[i].thread_id == expected[i].thread_id);
		BOOST_CHECK(ret[i].post_id == expected[i].post_id);
		BOOST_CHECK(ret[i].content == expected[i].content);
		BOOST_CHECK(ret[i].img_url == expected[i].img_url);
		BOOST_CHECK(ret[i].img == expected[i].img);
	}
}

BOOST_AUTO_TEST_CASE(check_parse_threads) {

	chan_parser p;

	std::vector<std::vector<chan_post>> expected({
{
{"test", "2374", "2374", "1356806391743.png", "http://desuchan.net/tech/src/1356806391743.png", "What are you working on?"},
{"test", "2374", "2375", "1356867546959.jpg", "http://desuchan.net/tech/src/1356867546959.jpg", "NOTHING NOTHING AT ALL"},
{"test", "2374", "2378", "", "", "A java fizzbuzz program"},
{"test", "2374", "2400", "1369856779897.jpg", "http://desuchan.net/tech/src/1369856779897.jpg", ">>2378 Tryin` to make some shit in Python. Now it`s MAL parser. http://rghost.net/46363268"},
{"test", "2374", "2404", "1372814015705.jpg", "http://desuchan.net/tech/src/1372814015705.jpg", ">>2374 nothing"},
{"test", "2374", "2411", "1376735987000.jpg", "http://desuchan.net/tech/src/1376735987000.jpg", "figuring out how to make access with vba scripting take an exported excel file, convert it to a drop down table, and email it, just at the press of a button... because the people i work with are too stupid to attach a file and send an email themselves Last edited 13/08/17(Sat)06:39."},
}, 
{
{"test", "1591", "1591", "1268170490717.jpg", "http://desuchan.net/tech/src/1268170490717.jpg", "Post your cell phone."},
{"test", "1591", "2347", "1352567184741.jpg", "http://desuchan.net/tech/src/1352567184741.jpg", "INQ Chat 3G Got this one a while back, and honestly I am going to argue it is the best phone for the dollar. It was $50, but when you bought it you get a $25 telus card, which more or less made the phone $25 dollars. The camera has auto focus  which makes its pictures embarassingly better than my Ipod touch's. The videos it takes on the other hand are more suited for filming bigfoot or the lochness monster.  Its screen is large enough, the qwerty keyboard is nice, and I have very good reception with it."},
{"test", "1591", "2348", "", "", ">>2166 If shes better then a 6/10 and you aren't posting the pics you are a cold bastard."},
{"test", "1591", "2349", "1352601714758.jpg", "http://desuchan.net/tech/src/1352601714758.jpg", "Running MIUI 2.11.9, with some mods and my own kernel. Not bad for a 2 year old single core. Last edited 12/11/10(Sat)21:41."},
{"test", "1591", "2352", "", "", ">>2348 I hope for his sake it vvas a \"she\" D:"},
{"test", "1591", "2409", "1373709181341.jpg", "http://desuchan.net/tech/src/1373709181341.jpg", "After two years with my galaxy S2, I decided it'd be nice to go for an S4. Can't complain, this is a great phone so far."},
}, 
{
{"test", "2346", "2346", "1352115443938.jpg", "http://desuchan.net/tech/src/1352115443938.jpg", ">>2320 I prefer the BRZ http://gt86.com.au/subaru-brz/ http://subaru.com.au/brz"},
{"test", "2346", "2390", "1361183422885.jpg", "http://desuchan.net/tech/src/1361183422885.jpg", ">>2368 <3 that Promo"},
{"test", "2346", "2395", "1362394120143.jpg", "http://desuchan.net/tech/src/1362394120143.jpg", "Toyota 86 convertible http://news.drive.com.au/drive/motor-news/toyota-86-convertible-revealed-20130301-2faat.html"},
{"test", "2346", "2402", "1370866849946.jpg", "http://desuchan.net/tech/src/1370866849946.jpg", "Subaru have annouced the production of the Subaru BRZ S edition, due to be released for sale June 2013 and anticipated to cost a little more than $40,000 on road.  The major differences are new Look front bumpers and side skirts, different boot lip spoiler and supporting 17\" black STI rims. Some additional add-on extras now available are  * Reversing Camera  * Satelitte Navigation  * STI Sports Exhaust  * Front / Rear Parking Sensors http://news.drive.com.au/drive/motor-news/sportier-subaru-brz-confirmed-20130527-2n6r6.html"},
{"test", "2346", "2403", "1371976709026.jpg", "http://desuchan.net/tech/src/1371976709026.jpg", "Custom-made V8 BRZ06! http://www.reddit.com/r/cars/comments/1gqees/pictures_story_of_the_ls2_v8swapped_subaru_brz/"},
{"test", "2346", "2408", "1373543235480.jpg", "http://desuchan.net/tech/src/1373543235480.jpg", "Koenigseggeggeggegg http://www.therefinedgeek.com.au/index.php/2013/07/10/koenigseggs-amazing-technology/"},
}, 
{
{"test", "2291", "2291", "1329490398893.jpg", "http://desuchan.net/tech/src/1329490398893.jpg", "https://www.youtube.com/watch?v=F1uQsgeGK8s"},
{"test", "2291", "2394", "1362046357902.jpg", "http://desuchan.net/tech/src/1362046357902.jpg", ">>2393"},
{"test", "2291", "2396", "", "", "Avalon Airshow 2013 http://www.flickr.com/photos/craigs1/sets/72157632844674862/ http://www.flickr.com/photos/lockheedmartin/sets/72157632882629290/with/8516472252/"},
{"test", "2291", "2405", "1373175362738.jpg", "http://desuchan.net/tech/src/1373175362738.jpg", "Soooooniiiiiiiiiiiiic BOOM http://www.bbc.com/future/story/20130701-tales-from-the-blackbird-cockpit/all"},
{"test", "2291", "2406", "1373195222862.jpg", "http://desuchan.net/tech/src/1373195222862.jpg", "http://www.washingtonpost.com/politics/solar-powered-plane-has-tear-on-wing-earlier-landing-expected-in-nyc-on-final-leg-of-flight/2013/07/06/98e50c56-e69a-11e2-97c2-eee7eab7397e_story.html"},
{"test", "2291", "2407", "1373458957157.jpg", "http://desuchan.net/tech/src/1373458957157.jpg", "Duxford Imperial War Museum located near Cambridge in the UK http://forums.overclockers.com.au/showthread.php?t=1091372"},
}, 
{
{"test", "2365", "2365", "1354620590115.jpg", "http://desuchan.net/tech/src/1354620590115.jpg", "How fares your tech for 2012? http://www.abc.net.au/technology/articles/2012/11/29/3643758.htm"},
{"test", "2365", "2373", "1356743773248.jpg", "http://desuchan.net/tech/src/1356743773248.jpg", "http://theoatmeal.com/comics/state_web_summer"},
{"test", "2365", "2379", "1358072497276.jpg", "http://desuchan.net/tech/src/1358072497276.jpg", "2012? You're living in the past, man... http://hardware.slashdot.org/story/13/01/11/2119227/chinese-smartphone-invasion-begins"},
{"test", "2365", "2398", "1367214814344.jpg", "http://desuchan.net/tech/src/1367214814344.jpg", ">>2379 That I am"},
}, 
{
{"test", "2213", "2213", "1313349453957.jpg", "http://desuchan.net/tech/src/1313349453957.jpg", "Currently this is my battlestation. A desktop that is a real desktop, not one of those limited gimped mini desktop. A three-year old laptop that is currently unused for now but I want to use it for secondary purposes and stuff like for DVD viewing and shit. A 2.1. sound system A simple headphone with mic. But I want to add the following for my perfect battlestation: Two other monitors. I have one other monitor, which is a 720p 16:10 HDTV/DVD player combo in storage and want to buy another 16:9 monitor as well. Also I want to add two vertical monitors for texts such as magazines and digital books and irc conversations/windows. Hook up my modern video game consoles (PS2, Xbox 360) to my battlestation Possibly another external hard drive for all my stored anime, music, and recorded TV shows, and have my current hard drive I have to be used as a static archive/recovery hard drive, and get a new external hard drive for my current content. I've been waiting to do this since early 2010, and this is my chance. Any other suggestions in how this can be done, feel free to reply."},
{"test", "2213", "2288", "1327737009028.jpg", "http://desuchan.net/tech/src/1327737009028.jpg", "Triple head"},
{"test", "2213", "2301", "1335608353955.jpg", "http://desuchan.net/tech/src/1335608353955.jpg", ""},
{"test", "2213", "2377", "", "", "I want more monitors!"},
{"test", "2213", "2387", "", "", "Oooooh... I wants it! http://desuchan.net/v/res/1882.html#i7047"},
{"test", "2213", "2389", "1360058934338.jpg", "http://desuchan.net/tech/src/1360058934338.jpg", "I need more monitors to improve my productivity! http://www.apartmenttherapy.com/roundup-multiple-monitor-homes-52915 http://superuser.com/questions/9034/multi-monitor-usage"},
}, 
{
{"test", "2376", "2376", "1356997088622.jpg", "http://desuchan.net/tech/src/1356997088622.jpg", "Sciene salutes its fallen. http://www.washingtonpost.com/national/health-science/rita-levi-montalcini-nobel-prize-winning-neuroscientist-dies-at-103/2012/12/30/054829e0-5295-11e2-a613-ec8d394535c6_story.html"},
{"test", "2376", "2381", "1358163203595.jpg", "http://desuchan.net/tech/src/1358163203595.jpg", "Instruments of sciene will stand tall in the face of disaster. http://www.abc.net.au/news/2013-01-14/homes-destroyed-in-nsw-bushfire/4463136"},
}, 
{
{"test", "2361", "2361", "1354619159093.jpg", "http://desuchan.net/tech/src/1354619159093.jpg", "Cooking is basically chemistry, right?"},
{"test", "2361", "2362", "1354619213741.jpg", "http://desuchan.net/tech/src/1354619213741.jpg", "I present to you..... http://www.instructables.com/id/Bacon-Jello/?ALLSTEPS Bacon Jello. YEAH!"},
{"test", "2361", "2366", "", "", "Tiramisu.. I choose you! http://m.notquitenigella.com/2012/11/30/cloud-9-black-forest-tiramisu/"},
{"test", "2361", "2372", "1356438706176.jpg", "http://desuchan.net/tech/src/1356438706176.jpg", "I rather leave the 'chemistry' to others http://charpost.blogspot.com.au/2012/09/wei-kee-chinese-restaurant-rochester-st.html"},
{"test", "2361", "2380", "1358072876569.jpg", "http://desuchan.net/tech/src/1358072876569.jpg", ">>2361 Yes. http://n2extremegelato.com.au/"},
}, 
{
{"test", "2139", "2139", "1300790520864.png", "http://desuchan.net/tech/src/1300790520864.png", "Writers, distribute thyself! http://jakonrath.blogspot.com/2011/03/ebooks-and-self-publishing-dialog.html"},
{"test", "2139", "2163", "1303776893844.jpg", "http://desuchan.net/tech/src/1303776893844.jpg", "Anime, distribute thyself! http://arstechnica.com/tech-policy/news/2011/04/competing-with-free-anime-site-treats-piracy-as-a-market-failure.ars"},
{"test", "2139", "2309", "1338784881755.jpg", "http://desuchan.net/tech/src/1338784881755.jpg", "Cheers! http://www.wizards.com/Magic/Magazine/Article.aspx?x=mtg/daily/mm/198"},
{"test", "2139", "2358", "1354014551374.jpg", "http://desuchan.net/tech/src/1354014551374.jpg", "Self-promotion & Distribution is easier with the internet... See \"50 shades of grey\""},
{"test", "2139", "2360", "1354330341596.jpg", "http://desuchan.net/tech/src/1354330341596.jpg", "World of GOO http://venturebeat.com/2009/01/02/the-world-of-goo-became-one-of-the-indie-hits-of-2008/"},
}, 
{
{"test", "2305", "2305", "1338099904487.jpg", "http://desuchan.net/tech/src/1338099904487.jpg", "Anyone here own a tablet?"},
{"test", "2305", "2307", "1338656424342.jpg", "http://desuchan.net/tech/src/1338656424342.jpg", "I bought an Ainol Paladin.  It is a Chinese knockoff tablet that runs Android 4, but it is really not that bad."},
}
});
	std::string xml = dump_file("list_page.html");

	std::vector<std::vector<chan_post>> ret = p.parse_threads("test", xml);

	BOOST_CHECK(ret.size() >= expected.size());
	for (unsigned j = 0; j < expected.size(); j++) {

		BOOST_CHECK(ret[j].size() >= expected[j].size());
		for (unsigned i = 0; i < expected[j].size(); i++) {

			BOOST_CHECK(ret[j][i].thread_id == expected[j][i].thread_id);
			BOOST_CHECK(ret[j][i].post_id == expected[j][i].post_id);
			BOOST_CHECK(ret[j][i].content == expected[j][i].content);
			BOOST_CHECK(ret[j][i].img_url == expected[j][i].img_url);
			BOOST_CHECK(ret[j][i].img == expected[j][i].img);
		}
	}
}
