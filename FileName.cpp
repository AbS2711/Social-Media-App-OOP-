#include <iostream>
#include <string>
using namespace std;

// Forward class declarations
class Entity;
class User;
class Page;
class Post;
class Comment;
class Activity;
class Memory;
class SocialNetworkApp;


// Entity class implementation
class Entity {
protected:
    string id;
public:
    Entity(string id) : id(id) {}
    virtual ~Entity() {}
    string getId() const { return id; }
};

// Comment class implementation
class Comment : public Entity {
private:
    Entity* author;
    string text;
public:
    Comment(string id, Entity* author, string text) : Entity(id), author(author), text(text) {}

    void viewDetails() const {
        cout << " Wrote: " << text << endl;
    }
};

// Post class implementation
class Post : public Entity {
protected:
    string content;
    int likeCount;
    User** likedBy;
    int likedByCount;
    int likedByCapacity;
    string date;
    Comment** comments;
    int commentCount;
    int commentCapacity;
public:
    Post(string id, string content, string date) : Entity(id), content(content), date(date), likeCount(0), likedByCount(0), likedByCapacity(10), commentCount(0), commentCapacity(10) {
        likedBy = new User * [likedByCapacity];
        comments = new Comment * [commentCapacity];
    }

    virtual ~Post() {
        delete[] likedBy;
        for (int i = 0; i < commentCount; i++) {
            delete comments[i];
        }
        delete[] comments;
    }

    void addLike(User* user) {
        if (likedByCount < likedByCapacity) {
            likedBy[likedByCount++] = user;
            likeCount++;
        }
        else {
            cout << "Like limit reached for this post." << endl;
        }
    }
    void viewLikes();

    void addComment(Comment* comment) {
        if (commentCount < commentCapacity) {
            comments[commentCount++] = comment;
        }
        else {
            cout << "Comment limit reached for this post." << endl;
        }
    }

    void viewComments() const {
        cout << "Comments on post " << id << ":" << endl;
        for (int i = 0; i < commentCount; i++) {
            cout << '\t' << comments[i]->getId();
            comments[i]->viewDetails();
        }
        if (commentCount == 0) {
            cout << "No comments yet." << endl;
        }
    }

    virtual void viewDetails() const {
        cout << content << endl;

        viewComments();
        cout << "----------------------------------------------" << endl;

    }

    string getContent() const { return content; }
    string getDate() const { return date; }
};

class Page : public Entity {
private:
    string title;
    int likeCount;
    Post** posts;
    int postCount;
    int postCapacity;
public:
    Page(string id, string title) : Entity(id), title(title), likeCount(0), postCount(0), postCapacity(10) {
        posts = new Post * [postCapacity];
    }

    ~Page() {
        for (int i = 0; i < postCount; i++) {
            delete posts[i];
        }
        delete[] posts;
    }

    void addPost(Post* post) {
        if (postCount < postCapacity) {
            posts[postCount++] = post;
        }
        else {
            cout << "Post limit reached for this page." << endl;
        }
    }
    void addComment(string postId, Comment* comment) {
        for (int i = 0; i < postCount; i++) {
            if (posts[i]->getId() == postId) {
                posts[i]->addComment(comment);
                return;
            }
        }
        cout << "Post with ID " << postId << " not found on this page." << endl;
    }



    void like() {
        likeCount++;
    }

    void viewPosts() const {
        cout << "Posts on " << title << " (" << id << "):" << endl;
        for (int i = 0; i < postCount; i++) {
            posts[i]->viewDetails();
        }
        if (postCount == 0) {
            cout << "No posts yet." << endl;
        }
    }

    string getTitle() const { return title; }
    int getLikeCount() const { return likeCount; }
};


// Activity class implementation
class Activity : public Post {
private:
    string activityType;
    string activityValue;
public:
    Activity(string id, string content, string date, string activityType, string activityValue) : Post(id, content, date), activityType(activityType), activityValue(activityValue) {
    }

    void viewDetails() const override {
        Post::viewDetails();
        cout << activityType << ": ";
        cout << activityValue << endl;
    }
};
// User class implementation
class User : public Entity {
private:
    string name;
    User** friends;
    int friendCount;
    int friendCapacity;
    Page** likedPages;
    int likedPageCount;
    int likedPageCapacity;
    Post** timeline;
    int postCount;
    int postCapacity;
public:
    User(string id, string name) : Entity(id), name(name), friendCount(0), friendCapacity(10), likedPageCount(0), likedPageCapacity(10), postCount(0), postCapacity(10) {
        friends = new User * [friendCapacity];
        likedPages = new Page * [likedPageCapacity];
        timeline = new Post * [postCapacity];
    }

    ~User() {
        delete[] friends;
        delete[] likedPages;
        for (int i = 0; i < postCount; i++) {
            delete timeline[i]; // delete each Post*
        }
        delete[] timeline;
    }

    void addFriend(User* user) {
        if (friendCount < friendCapacity) {
            friends[friendCount++] = user;
        }
        else {
            cout << "Friend limit reached for this user." << endl;
        }
    }

    void likePage(Page* page) {
        if (likedPageCount < likedPageCapacity) {
            likedPages[likedPageCount++] = page;
            page->like();
        }
        else {
            cout << "Page limit reached for this user." << endl;
        }
    }

    void addPost(Post* post) {
        if (postCount < postCapacity) {
            timeline[postCount++] = post;
        }
        else {
            cout << "Post limit reached for this user." << endl;
        }
    }

    void viewFriends() const {
        cout << "Friends of " << name << " (" << id << "):" << endl;
        for (int i = 0; i < friendCount; i++) {
            cout << "- " << friends[i]->name << " (" << friends[i]->id << ")" << endl;
        }
        if (friendCount == 0) {
            cout << "No friends yet." << endl;
        }
    }

    void viewLikedPages() const {
        cout << "Liked pages of " << name << " (" << id << "):" << endl;
        for (int i = 0; i < likedPageCount; i++) {
            cout << "- " << likedPages[i]->getId() << ": " << likedPages[i]->getTitle() << endl;

        }
        if (likedPageCount == 0) {
            cout << "No liked pages yet." << endl;
        }
    }

    void viewTimeline() const {
        cout << "Timeline of " << name << " (" << id << "):" << endl;
        for (int i = 0; i < postCount; i++) {
            timeline[i]->viewDetails();
        }
        if (postCount == 0) {
            cout << "No posts yet." << endl;
        }
    }

    string getName() const { return name; }

    void addActivity(int type, const string& value) {
        string activityType;
        string activityValue = value;

        switch (type) {
        case 1:
            activityType = "feeling";
            // value: Happy/sad/excited etc.
            break;
        case 2:
            activityType = "thinking about";
            // value: life/future/meaning of life etc.
            break;
        case 3:
            activityType = "Making";
            // value: Money/art/memories etc.
            break;
        case 4:
            activityType = "celebrating";
            // value: A birthday/Halloween/success etc.
            break;
        default:
            cout << "Invalid activity type." << endl;
            return;
        }

        // Compose content for the activity post
        string content = activityType + " " + activityValue;

        // Use current date as activity date (assuming today's date for demonstration)
        string date = "15/11/2017";

        // Generate a unique id for the activity post (for demonstration, use name + type + value)
        string id = getId() + "activity" + to_string(type);

        // Create and add the activity post to the user's timeline
        Activity* activity = new Activity(id, content, date, activityType, activityValue);
        addPost(activity);
    }
    void getFriendPosts() const {
        cout << "Posts from friends of " << name << " (" << id << "):" << endl;
        for (int i = 0; i < friendCount; i++) {
            friends[i]->viewTimeline();
            cout << "----------------------------------------" << endl;

        }
    }
    void getLikedPagePosts() const {
        for (int i = 0; i < likedPageCount; i++) {
            likedPages[i]->viewPosts();
            cout << "----------------------------------------" << endl;
        }
    }
    void addCommentToPost(string postId, string commentId, string text) {
        for (int i = 0; i < postCount; i++) {
            if (timeline[i]->getId() == postId) {
                Comment* comment = new Comment(commentId, this, text);
                timeline[i]->addComment(comment);
                return;
            }
        }
        cout << "Post not found." << endl;
    }
};






// Memory class implementation
class Memory : public Post {
private:
    Post* originalPost;
public:
    Memory(string id, string content, string date, Post* originalPost) : Post(id, content, date), originalPost(originalPost) {
    }

    void viewDetails() const override {
        cout << "--- Memory ---" << endl;
        Post::viewDetails();
        cout << "Original Post:" << endl;
        originalPost->viewDetails();
    }
};

// SocialNetworkApp class implementation
class SocialNetworkApp {
private:
    User** users;
    int userCount;
    int userCapacity;
    Page** pages;
    int pageCount;
    int pageCapacity;
    Post** posts;
    int postCount;
    int postCapacity;
    User* loggedInUser;
    string currentDate;
public:
    SocialNetworkApp() : userCount(0), userCapacity(10),
        pageCount(0), pageCapacity(10),
        postCount(0), postCapacity(10),
        loggedInUser(nullptr) {
        users = new User * [userCapacity];
        pages = new Page * [pageCapacity];
        posts = new Post * [postCapacity];
    }

    ~SocialNetworkApp() {}

    void setCurrentDate(string date) {
        currentDate = date;
    }

    void addUser(User* user) {
        if (userCount < userCapacity) {
            users[userCount++] = user;
        }
        else {
            cout << "User limit reached." << endl;
        }
    }

    void addPage(Page* page) {
        if (pageCount < pageCapacity) {
            pages[pageCount++] = page;
        }
        else {
            cout << "Page limit reached." << endl;
        }
    }

    void addPost(Post* post) {
        if (postCount < postCapacity) {
            posts[postCount++] = post;
        }
        else {
            cout << "Post limit reached." << endl;
        }
    }

    void setCurrentUser(string userId) {
        for (int i = 0; i < userCount; i++) {
            if (users[i]->getId() == userId) {
                loggedInUser = users[i];
                cout << "Logged in as: " << users[i]->getName() << endl;
                return;
            }
        }
        cout << "User not found." << endl;
    }

    void viewHome() const {
        if (loggedInUser) {
            cout << "=== Home Page ===" << endl;
            cout << "Welcome, " << loggedInUser->getName() << "!" << endl << endl;

            loggedInUser->getFriendPosts();
            loggedInUser->getLikedPagePosts();
            cout << "Your Timeline:" << endl;


        }
        else {
            cout << "No user logged in." << endl;
        }
    }

    void likePost(string postId) {
        if (!loggedInUser) {
            cout << "No user logged in." << endl;
            return;
        }

        for (int i = 0; i < postCount; i++) {
            if (posts[i]->getId() == postId) {
                posts[i]->addLike(loggedInUser);
                return;
            }
        }
        cout << "Post not found." << endl;
    }

    void commentOnPost(string postId, string commentId, string text) {
        if (!loggedInUser) {
            cout << "No user logged in." << endl;
            return;
        }

        for (int i = 0; i < postCount; i++) {
            if (posts[i]->getId() == postId) {
                Comment* comment = new Comment(commentId, loggedInUser, text);
                posts[i]->addComment(comment);
                return;
            }
        }
        cout << "Post not found." << endl;
    }

    void shareMemory(string postId, string memoryId, string content) {
        if (!loggedInUser) {
            cout << "No user logged in." << endl;
            return;
        }

        for (int i = 0; i < postCount; i++) {
            if (posts[i]->getId() == postId) {
                Memory* memory = new Memory(memoryId, content, currentDate, posts[i]);
                loggedInUser->addPost(memory);
                addPost(memory);
                return;
            }
        }
        cout << "Original post not found." << endl;
    }

    void viewTimeline(string userId) const {
        for (int i = 0; i < userCount; i++) {
            if (users[i]->getId() == userId) {
                cout << "=== Timeline of " << users[i]->getName() << " ===" << endl;
                users[i]->viewTimeline();
                cout << "-------------------------------------------------------" << endl;
                return;
            }
        }
        cout << "User not found." << endl;
    }

    void viewPage(string pageId) const {
        for (int i = 0; i < pageCount; i++) {
            if (pages[i]->getId() == pageId) {
                cout << "=== Page: " << pages[i]->getTitle() << " ===" << endl;
                cout << "Page ID: " << pages[i]->getId() << endl;
                cout << "Likes: " << pages[i]->getLikeCount() << endl;
                cout << "\nPosts:" << endl;
                pages[i]->viewPosts();
                return;
            }
        }
        cout << "Page not found." << endl;
    }
    void viewLikedList(string postId) const {
        for (int i = 0; i < postCount; i++) {
            if (posts[i]->getId() == postId) {
                posts[i]->viewLikes();
                return;
            }
        }
        cout << "Post not found." << endl;
    }

    void registerUser(string id, string name) {
        if (userCount < userCapacity) {
            users[userCount++] = new User(id, name);
        }
        else {
            cout << "User limit reached." << endl;
        }
    }

    void createPage(string id, string title) {
        if (pageCount < pageCapacity) {
            pages[pageCount++] = new Page(id, title);
        }
        else {
            cout << "Page limit reached." << endl;
        }
    }

    void createPost(string id, string content, string authorId, bool isPage) {

        if (postCount < postCapacity) {
            Post* post = new Post(id, content, currentDate);

            if (isPage) {
                for (int i = 0; i < pageCount; i++) {
                    if (pages[i]->getId() == authorId) {
                        pages[i]->addPost(post);
                        addPost(post);
                        return;
                    }
                }
                cout << "Page not found." << endl;
                delete post;
            }
            else {
                for (int i = 0; i < userCount; i++) {
                    if (users[i]->getId() == authorId) {
                        users[i]->addPost(post);
                        addPost(post);
                        return;
                    }
                }
                cout << "User not found." << endl;
                delete post;
            }
        }
        else {
            cout << "Post limit reached." << endl;
        }
    }

    void createActivity(string id, string content, string authorId, string activityType, string activityValue, bool isPage) {
        if (postCount < postCapacity) {
            Activity* activity = new Activity(id, content, currentDate, activityType, activityValue);

            if (isPage) {
                for (int i = 0; i < pageCount; i++) {
                    if (pages[i]->getId() == authorId) {
                        pages[i]->addPost(activity);
                        addPost(activity);
                        return;
                    }
                }
                cout << "Page not found." << endl;
                delete activity;
            }
            else {
                for (int i = 0; i < userCount; i++) {
                    if (users[i]->getId() == authorId) {
                        users[i]->addPost(activity);
                        addPost(activity);
                        return;
                    }
                }
                cout << "User not found." << endl;
                delete activity;
            }
        }
        else {
            cout << "Post limit reached." << endl;
        }
    }
};
void Post::viewLikes() {
    cout << "Liked by:" << endl;
    for (int i = 0; i < likedByCount; i++) {
        cout << "- " << likedBy[i]->getId() << endl;
    }
    if (likedByCount == 0) {
        cout << "No likes yet." << endl;
    }
}

void runSocialNetworkApp() {

    SocialNetworkApp app;

    // Command: Set current System Date
    cout << "Command: Set current System Date 15 11 2017" << endl;
    app.setCurrentDate("15/11/2017");
    cout << "System Date: 15/11/2017\n" << endl;

    // Users
    User* u2 = new User("u2", "Amna Akram");
    User* u3 = new User("u3", "Waheed Khan");
    User* u4 = new User("u4", "Burhan Tahir");
    User* u7 = new User("u7", "Abdullah Nadeem");
    User* u11 = new User("u11", "Ahsan Ali");
    User* u12 = new User("u12", "Ali Ahmed");

    app.addUser(u2);
    app.addUser(u3);
    app.addUser(u4);
    app.addUser(u7);
    app.addUser(u11);
    app.addUser(u12);

    // Pages
    Page* p1 = new Page("P1", "Food Fusion");
    Page* p2 = new Page("P2", "Al Jazeera English");
    Page* p5 = new Page("P5", "Rumi Quotes");
    Page* p9 = new Page("P9", "BBC News");

    app.addPage(p1);
    app.addPage(p2);
    app.addPage(p5);
    app.addPage(p9);

    // Friendships
    u7->addFriend(u3);
    u7->addFriend(u11);
    u7->addFriend(u12);

    //adding posts to friends

    u3->addActivity(4, "celebrating success");
    u3->addPost(new Post("post1", "OOP Project Started Yayy...", "15/11/2017"));
    u11->addPost(new Post("post2", "Waiting for Weekend", "15/11/2017"));
    u11->addPost(new Post("post3", "Happy Birthday Dear MOM!!!", "15/11/2017"));
    u11->addPost(new Post("post4", "Waiting for Summer holidays", "15/11/2017"));
    u12->addPost(new Post("post4", "Waiting for Result", "15/11/2017"));
    //adding comments to friends
    u3->addCommentToPost("post1", "Waheed Khan", "Best of Luck");
    u3->addCommentToPost("post1", "Waheed Khan", "Interesting");
    u11->addCommentToPost("post2", "Ahsan Ali", "Any plans for Get Together?");
    u11->addCommentToPost("post2", "Ahsan Ali", "Yes BBQ");
    u11->addCommentToPost("post2", "Ahsan Ali", "Lunch Buffet please");
    u11->addCommentToPost("post3", "Ahsan Ali", "Happy Birthday to your Mom");
    u11->addCommentToPost("post3", "Deez", "Wish her from my side");
    u11->addCommentToPost("post3", "Amna", "May she live long");
    u11->addCommentToPost("post4", "Ahsan Ali", "Good Luck for your Result");
    u11->addCommentToPost("post4", "Ahsan Ali", "Best of Luck for your Result");

    u12->addCommentToPost("post4", "Ahsan Ali", "Followers please try this recipe and give your feedback");



    // Adding posts to pages
    p1->addPost(new Post("post6", "Try this at home...", "15/11/2017"));
    p2->addPost(new Post("post7", "Oil drilling in Alaska's indigenous communities is on the table again.", "15/11/2017"));
    p5->addPost(new Post("post5", "Be free of possessing things - Rumi", "15/11/2017"));
    p9->addPost(new Post("post8", "Happy Birthday to my dear Mom", "15/11/2017"));

    p1->addComment("post6", new Comment("Amna Akram", u2, "Amazing Recipe"));
    p2->addComment("post7", new Comment("Amna Akram", u2, "Interesting"));
    p5->addComment("post5", new Comment("Burhan Tahir", u4, "Beautiful Quote"));
    p9->addComment("post8", new Comment("Burhan Tahir", u4, "Happy Birthday!!!"));
    // Adding likes to posts
    u3->likePage(p1);
    u3->likePage(p2);
    u3->likePage(p5);
    u3->likePage(p9);
    u11->likePage(p1);
    u11->likePage(p2);
    u11->likePage(p5);
    u11->likePage(p9);
    u12->likePage(p1);
    u12->likePage(p2);
    u12->likePage(p5);
    u12->likePage(p9);



    // Liked Pages
    u7->likePage(p1);
    u7->likePage(p2);
    u7->likePage(p5);
    u7->likePage(p9);

    //add to timeline
    u7->addPost(new Post("post9", "Happy Birthday to my dear Father", "15/11/2017"));
    u7->addPost(new Post("post10", "Got D in toughest subject, Thank God I finally passed!!!", "15/11/2017"));

    u7->addCommentToPost("post9", "Abdullah Nadeem", "Happy Birthday Uncle");

    u7->addCommentToPost("post10", "Abdullah Nadeem", "Celebrations???");


    // Command: Set current user u7
    cout << "Command: Set current user u7" << endl;
    app.setCurrentUser("u7");
    cout << "Abdullah Nadeem successfully set as Current User\n" << endl;

    // Command: View Friend List
    cout << "Command: View Friend List" << endl;
    cout << "-------------------------------------------------------------------------------------------------------------" << endl;
    cout << "Abdullah Nadeem's Friend List" << endl;
    u7->viewFriends();
    cout << "---------------------------------------------------------------------------------------------------------------\n" << endl;

    // Command: View Liked Pages
    cout << "Comand: View Liked Pages" << endl;
    cout << "---------------------------------------------------------------------------------------------------------------" << endl;
    cout << "Abdullah Nadeem's Liked Pages" << endl;
    u7->viewLikedPages();
    cout << "---------------------------------------------------------------------------------------------------------------\n" << endl;
    // Command: View Home
    cout << "Command: View Home" << endl;
    cout << "---------------------------------------------------------------------------------------------------------------" << endl;
    app.viewHome();
    cout << "---------------------------------------------------------------------------------------------------------------\n" << endl;
    // Command: View Timeline
    cout << "Command: View Timeline" << endl;
    cout << "---------------------------------------------------------------------------------------------------------------" << endl;
    app.viewTimeline("u7");
    cout << "---------------------------------------------------------------------------------------------------------------\n" << endl;

    cout << "Command: View liked list of post5" << endl;
    cout << "---------------------------------------------------------------------------------------------------------------" << endl;
    app.viewLikedList(p5->getId());


    //// Add likes on post5
    //app.likePost("post5"); // u7 likes
    //u2->addPost(nullptr); // simulate presence
    //u3->addPost(nullptr);
    //u4->addPost(nullptr);
    //app.likePost("post5"); // add other likes manually
    //app.likePost("post5");
    //app.likePost("post5");

    //// View Liked List before and after
    //cout << "Command: ViewLikedList(post5)\n" << endl;
    //cout << "Post Liked By:\nU2 Amna Akram\nP1 Food Fusion\nU3 Waheed Khan\nU4 Burhan Tahir\n" << endl;
    //cout << "Command: LikePost(post5)" << endl;
    //app.likePost("post5");
    //cout << "Command: ViewLikedList(post5)\n" << endl;
    //cout << "Post Liked By:\nU2 Amna Akram\nP1 Food Fusion\nU3 Waheed Khan\nU4 Burhan Tahir\nu7 Abdullah Nadeem\n" << endl;

    //// Comment on post4 and post8
    //cout << "Command: PostComment(post4, Good Luck for your Result)" << endl;
    //app.commentOnPost("post4", "c13", "Good Luck for your Result");

    //cout << "Command: ViewPost(post4)" << endl << endl;
    //app.viewTimeline("u12"); // includes post4 with comment

    //cout << "\nCommand: PostComment(post8, Thanks for the wishes)" << endl;
    //app.commentOnPost("post8", "c14", "Thanks for the wishes");

    //cout << "Command: ViewPost(post8)" << endl << endl;
    //app.viewTimeline("u7"); // includes post8 with new comment

    //// Share Memory
    //cout << "\nCommand: SeeYourMemories()\n\nOn this Day\n2 Years Ago\n";
    //app.shareMemory("post10", "post11", "Never thought I will be specialist in this field…");

    //// View updated timeline with memory
    //cout << "\nCommand: View TimeLine\n" << endl << endl;
    //app.viewTimeline("u7");

    //// View page p1
    //cout << "\nCommand: ViewPage(p1)" << endl << endl;
    //app.viewPage("P1");

    //// Set new current user
    //cout << "\nCommand: Set current user “u11”" << endl;
    //app.setCurrentUser("u11");
    //cout << "Ahsan Ali successfully set as Current User" << endl;

    //cout << "\nCommand: ViewHome" << endl << endl;
    //app.viewHome();

    //cout << "\nCommand: ViewTimeline" << endl << endl;
    //app.viewTimeline("u11");

}

int main() {
    runSocialNetworkApp();
    return 0;
}