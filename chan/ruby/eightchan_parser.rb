require 'rubygems'
require 'nokogiri'

def parse_post(post)

    r_post_id = ''
    r_file_ids = []
    r_file_urls = []
    r_content = ''

    # Two subnodes match the id "post_no", the first is "No." and the
    # second is the actual post number.
    post_id = post.search('a.post_no')[1]
    r_post_id = post_id.text

    files = post.search('p.fileinfo')
    files.each do |file|

        url = file.search('a').first
        r_file_ids.push(url.text)
        r_file_urls.push(url['href'])

        #(57.44 KB, 500x500, 1:1, 1423521168692.png)
        #meta = file.search('span.unimportant').first

        # Rei.jpg
        #post_filename = file.search('span.postfilename')
    end

    content = post.search('div.body')
    r_content = content.text

    return Chan_post_multi.new(
        "???", "???", r_post_id, r_file_ids, r_file_urls, r_content)
end

# Given an array of posts, extract the post id of the first (OP) and set it as the
# thread_id field of all the posts in the array.
def set_thread_ids(posts)
    
    return posts if posts.empty?
    
    thread_id = posts[0].get_post_id
    return posts.map do |post|
        post.set_thread_id(thread_id)
    end
end

def parse(str)

    doc = Nokogiri::HTML(str) do |config|
      config.noent.nonet
    end

    all_posts = []
    threads = doc.css('div.thread')
    threads.each do |thread|

        posts = thread.search('div.post')
        thread_posts = []

        posts.each do |post|
            thread_posts.push(parse_post(post))
        end
        
        thread_posts = set_thread_ids(thread_posts)
        all_posts.push(thread_posts.reverse)
    end

    return all_posts.reverse
end

def final_page?(str)
    return true
end
