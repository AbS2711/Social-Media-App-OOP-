#include <iostream>
#include <string>
using namespace std;
// Forward class declarations
class Entity; // main class which holds the unique id of each entity
class User; // inherits from the entity class, this has the name of the user, his friends and liked pages
class Page; // inherits from the entity class, this has the title of the page and posts
class Post; // inherits from the entity class, this has the content of the post, likes and comments
class Comment; // inherits from the entity class, this has the text of the comment and author   
class Activity; // inherits from the post class, this has the type of activity and value
class Memory; // inherits from the post class, this has the original post
class SocialNetworkApp; // main class which holds the users, pages and posts
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
// this class has view details function which shows the text of the comment and author
class Comment : public Entity {
private:
    Entity* author;
    string text;
public:
    Comment(string id, Entity* author, string text) : Entity(id), author(author), text(text) {}

    void viewDetails() const {
        cout << " Wrote: " << text << endl;
    }
	// DE ALLOCATED MEMORY AND MAKES SURE THAT NO MEMORY LEAKS OCCUR
	~Comment() {
		delete author;
	}
};

// Post class implementation
// it displays the comments, likes and content of the post
class Post : public Entity {
protected:
    string content;
    int likeCount;
    Entity** likedBy;
    int likedByCount;
    int likedByCapacity;
    string date;
    Comment** comments;
    int commentCount;
    int commentCapacity;
public:
    Post(string id, string content, string date) : Entity(id), content(content), date(date), likeCount(0), likedByCount(0), likedByCapacity(10), commentCount(0), commentCapacity(10) {
		likedBy = new Entity * [likedByCapacity]; // dynamic array of likedBy
		comments = new Comment * [commentCapacity]; // dynamic array of comments
    }
	// DE ALLOCATED MEMORY AND MAKES SURE THAT NO MEMORY LEAKS OCCUR
    virtual ~Post() {
        delete[] likedBy;
        for (int i = 0; i < commentCount; i++) {
            delete comments[i];
        }
        delete[] comments;
    }
    
    void addLike(Entity* user) {
        if (likedByCount < likedByCapacity) {
            likedBy[likedByCount++] = user;
            likeCount++;
        }
        else {
            cout << "Like limit reached for this post." << endl;
        }
    }

    void addComment(Comment* comment) {
        if (commentCount < commentCapacity) {
            comments[commentCount++] = comment;
        }
        else {
            cout << "Comment limit reached for this post." << endl;
        }
    }
    //displays with the user id, who liked the post
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

    //view details function which is overridden in memory and activity classes
	// this function shows the content of the post
    //it also shows the names of the people who commented on it and their respective comments

    virtual void viewDetails() const {
        cout << content << endl;
        viewComments();
        cout << "----------------------------------------------" << endl;
    }

    //forward function declaration
    void viewLikes();

    int getLikeCount() const { return likeCount; }
    string getContent() const { return content; }
    string getDate() const { return date; }
};
// Page class implementation
// this class has the title of the page and posts
// it displays the posts and likes of the page

class Page : public Entity {
private:
    string title;
    int likeCount;
    Post** posts;
    int postCount;
    int postCapacity;
public:
    Page(string id, string title) : Entity(id), title(title), likeCount(0), postCount(0), postCapacity(10) {
		posts = new Post * [postCapacity]; // dynamic array of posts using associative array
    }
	// DE ALLOCATED MEMORY AND MAKES SURE THAT NO MEMORY LEAKS OCCUR
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
    //accesses the post class and uses its methods to add the comment
    void addComment(string postId, Comment* comment) {
        for (int i = 0; i < postCount; i++) {
            if (posts[i]->getId() == postId) {
                posts[i]->addComment(comment);
                return;
            }
        }
        cout << "Post with ID " << postId << " not found on this page." << endl;
    }
	//increments the like count of the page
    void like() {
        likeCount++;
    }
    //this is used further into the code to display a specific post on the page along with all its commentss
	// it uses the post class to access the comments and likes
	void viewPostWithComments(string postId) const {
		for (int i = 0; i < postCount; i++) {
			if (posts[i]->getId() == postId) {
				posts[i]->viewDetails();
				return;
			}
		}
		cout << "Post with ID " << postId << " not found on this page." << endl;
	}
    //displays all the posts on a page
    //it loops through all the posts staying withi the post cout and then displays their details
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
// this class builds up on a regular post by adding an activity to it, from a specified pool of available activities
//further down the code, it is used to implement celebrating success
class Activity : public Post {
private:
    string activityType;
    string activityValue;
public:
    Activity(string id, string content, string date, string activityType, string activityValue) : Post(id, content, date), activityType(activityType), activityValue(activityValue) {
    }
    //overrides its prent function in post class to also outpt the activity type and its associated value
    void viewDetails() const override {
        Post::viewDetails();
        cout << activityType << ": ";
        cout << activityValue << endl;
    }
    // this entire functionality is passed down into the user class so that the user name can also be associated to the declaration
};
// User class implementation
// this class has the name of the user, his friends and liked pages
// it can also add actvities to the posts and view the liked pages and friends
//all these methods are accessed with regards to a specific user at that time

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
	// DE ALLOCATED MEMORY AND MAKES SURE THAT NO MEMORY LEAKS OCCUR
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
			timeline[i]->viewDetails(); // this displays all the posts of the user
        }
        if (postCount == 0) {
            cout << "No posts yet." << endl;
        }
    }

    string getName() const { return name; }

	//uses a switch case statement to see if the activity is valid or not
    void addActivity(int type, const string& value) {
        string activityType;
        string activityValue = value;
		//there were 4 types of activities available
        switch (type) {
        case 1:
            activityType = "feeling";
            
            break;
        case 2:
            activityType = "thinking about";
            
            break;
        case 3:
            activityType = "Making";
            
            break;
        case 4:
            activityType = "celebrating";
            
            break;
        default:
            cout << "Invalid activity type." << endl;
            return;
        }
		//concatenates  the activity type and value to create a content string
        string content = activityType + " " + activityValue;

        // Use current date as activity date (assuming today's date for demonstration)
        string date = "15/11/2017";

        // Generate a unique id for the activity post (for demonstration, use name + type + value)
        string id = getId() + "activity" + to_string(type);

        // Create and add the activity post to the user's timeline
        Activity* activity = new Activity(id, content, date, activityType, activityValue);
        addPost(activity);
		// Add the activity post to the app's posts (if needed)
    }
    //fetches all the posts on the friends timeline
    void getFriendPosts() const {
        cout << "Posts from friends of " << name << " (" << id << "):" << endl;
        for (int i = 0; i < friendCount; i++) {
            friends[i]->viewTimeline();
            cout << "----------------------------------------" << endl;

        }
    }
	//gets all the posts on the liked pages
    void getLikedPagePosts() const {
        for (int i = 0; i < likedPageCount; i++) {
            likedPages[i]->viewPosts();
            cout << "----------------------------------------" << endl;
        }
    }
    //uses the users id to post a comment on any post 
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
// this displays an old post 
class Memory : public Post {
private:
    Post* originalPost;
public:
    Memory(string id, string content, string date, Post* originalPost) : Post(id, content, date), originalPost(originalPost) {
    }
	//overrides the view details function to show the original post
    void viewDetails() const override {
        cout << "--- Memory ---" << endl;
        Post::viewDetails();
        cout << "Original Post:" << endl;
        originalPost->viewDetails();
    }
	~Memory() {
		delete originalPost; // Deallocate memory for the original post
	}
};

// SocialNetworkApp class implementation
//this is the class which handles most of the basic fundamental work
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
		users = new User * [userCapacity]; // dynamic array of users
		pages = new Page * [pageCapacity]; // dynamic array of pages
		posts = new Post * [postCapacity]; // dynamic array of posts
    }
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
    //sets the current user and displays stuff with his perspective
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
	//shows the timeline of logged in users friend and accesses all the post from his liked pages with respective comments for each
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
    //adds a like to a post with the name of the user also mentioned
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
    //uses his id to comment on any post
    void commentOnPost(string postId, string commentId, string text) {
        if (!loggedInUser) {
            cout << "No user logged in." << endl;
            return;
        }

        for (int i = 0; i < postCount; i++) {
			if (posts[i]->getId() == postId) {// to make sure that comment is only added to correct post we compare the post id in parameter to the specified post
                Comment* comment = new Comment(commentId, loggedInUser, text);
                posts[i]->addComment(comment);
                return;
            }
        }
        cout << "Post not found." << endl;
    }
	//accesses the memory class, created an object of it and adds it to the post
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
    //shows all the posts made by the user and the comments on it
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
	//shows all the posts made by the page and the comments on it
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
	//shows a specific post and its corresponding comments and likes
    void viewPost(string postId) const {
        for (int i = 0; i < postCount; i++) {
            if (posts[i]->getId() == postId) {
                cout << "Post ID: " << posts[i]->getId() << endl;
                cout << "Date: " << posts[i]->getDate() << endl;
                cout << "Content: " << posts[i]->getContent() << endl;

                // Show likes
                cout << "\nLikes (" << posts[i]->getLikeCount() << "):" << endl;
                posts[i]->viewLikes();

                // Show comments
                cout << "\nComments:" << endl;
                posts[i]->viewComments();
                return;
            }
        }
        cout << "Post not found." << endl;
    }
	//shows the likes on a specific post
    void viewLikedList(string postId) const {
        for (int i = 0; i < postCount; i++) {
            if (posts[i]->getId() == postId) {
                posts[i]->viewLikes();
                return;
            }
        }
        cout << "Post not found." << endl;
    }
    //creates a new user
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
    //creates a new post on a page
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
                    if (users[i]->getId() == authorId) {//compares if a valid user is adding comment
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
	// replaced with the createActivity method in user class
    /*void createActivity(string id, string content, string authorId, string activityType, string activityValue, bool isPage) {
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
    }*/
	
    //gets a specific post with the help of its id
    Post* getPost(string postId) const {
        for (int i = 0; i < postCount; i++) {
            if (posts[i]->getId() == postId) {
                return posts[i];
            }
        }
        return nullptr;
    }
};
// Function to view likes on a post
// this function is used to display the likes on a post
// it loops through the liked by array and checks if the entity is a user or a page
// if it is a user, it displays the name of the user and if it is a page, it displays the title of the page


//this basically checks if the post is of a user or a page by downcasting and confirming. Then it checks if there are any likes on that specific post or not
void Post::viewLikes() {
    cout << "Post Liked By:" << endl;
    for (int i = 0; i < likedByCount; i++) {
        Entity* entity = likedBy[i];
		User* user = dynamic_cast<User*>(entity);//downcasting to check if the entity is a user
        if (user) {
            cout << entity->getId() << " - " << user->getName() << endl;
        }
        else {
			Page* page = dynamic_cast<Page*>(entity);//downcasting to check if the entity is a page
            if (page) {
                cout << entity->getId() << " - " << page->getTitle() << endl;
            }
        }
    }
    if (likedByCount == 0) {
        cout << "No likes yet." << endl;
    }
}



int main() {

    SocialNetworkApp app;

    // Command: Set current System Date
    cout << "Command: Set current System Date 15 11 2017" << endl;
    app.setCurrentDate("15/11/2017");
    cout << "System Date: 15/11/2017\n" << endl;

    // Users that are registered
    User* u2 = new User("u2", "Amna Akram");
    User* u3 = new User("u3", "Waheed Khan");
    User* u4 = new User("u4", "Burhan Tahir");
    User* u7 = new User("u7", "Abdullah Nadeem");
    User* u11 = new User("u11", "Ahsan Ali");
    User* u12 = new User("u12", "Ali Ahmed");
    //registering users
    app.addUser(u2);
    app.addUser(u3);
    app.addUser(u4);
    app.addUser(u7);
    app.addUser(u11);
    app.addUser(u12);

    // Pages
    Page* p1 = new Page("P1", "Food Fusion");
    Page* p2 = new Page("P2", "Al Jazeera English");
    Page* p5 = new Page("p5", "Rumi Quotes");
    Page* p9 = new Page("P9", "BBC News");

    app.addPage(p1);
    app.addPage(p2);
    app.addPage(p5);
    app.addPage(p9);

    // Friendships of Abdullah Nadeem
    u7->addFriend(u3);
    u7->addFriend(u11);
    u7->addFriend(u12);

    //adding posts to friends

    u3->addActivity(4, " success");
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
    p1->addPost(new Post("post20", "Halwa puri nashta recipe", "15/12/2017"));
    p2->addPost(new Post("post7", "Oil drilling in Alaska's indigenous communities is on the table again.", "15/11/2017"));
    p5->addPost(new Post("post5", "Be free of possessing things - Rumi", "15/11/2017"));
    p9->addPost(new Post("post8", "Happy Birthday to my dear Mom", "15/11/2017"));

    p1->addComment("post6", new Comment("Amna Akram", u2, "Amazing Recipe"));
    p1->addComment("post6", new Comment("Waheed Khan", u3, "Yummy"));
    p1->addComment("post6", new Comment("Burhan Tahir", u4, "Delicious"));
    p1->addComment("post20", new Comment("Amna Akram", u2, "Amazing Recipe"));
    p1->addComment("post20", new Comment("Waheed Khan", u3, "Yummy"));

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

    Post* post10 = new Post("post10", "Got D in toughest subject, Thank God I finally passed!!!", "15/11/2017");
    u7->addPost(post10);
    app.addPost(post10);



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


    // Create post5 and add likes to it
    Post* post5 = new Post("post5", "Be free of possessing things - Rumi", "15/11/2017");
    p5->addPost(post5);
    app.addPost(post5);

    // Add initial likes
    post5->addLike(u2);  // Anna Akram
    post5->addLike(p1);  // Food Fusion
    post5->addLike(u3);  // Waheed Khan
    post5->addLike(u4);  // Burhan Tahir

    // Test ViewLikedList command
    cout << "Command: ViewLikedList(post5)" << endl;
    app.viewLikedList("post5");
    cout << endl;

    // Test LikePost command
    cout << "Command: LikePost(post5)" << endl;
    app.likePost("post5");
    cout << endl;

    // Test ViewLikedList again
    cout << "Command: ViewLikedList(post5)" << endl;
    app.viewLikedList("post5");
    cout << endl;
    cout << "---------------------------------------------------------------------------------------------------------------" << endl;
    cout << "Command: View Comments on post" << endl;
    cout << "---------------------------------------------------------------------------------------------------------------" << endl;
    p5->addComment("post5", new Comment("Amna Akram", u2, "Beautiful Quote"));
    p5->addComment("post5", new Comment("Burhan Tahir", u4, "Beautiful Quote"));
    p5->addComment("post5", new Comment("Waheed Khan", u3, "Beautiful Quote"));
    p5->viewPostWithComments("post5");
    cout << "---------------------------------------------------------------------------------------------------------------" << endl;
    cout << "Adding comment to post" << endl;
    p5->addComment("post5", new Comment("Abdullah Nadeem", u7, "I LOVE YOUR WORK FRFR"));
    p5->viewPostWithComments("post5");


    cout << "Command: see Memories" << endl;
    cout << "---------------------------------------------------------------------------------------------------------------" << endl;
    cout << "On this Day" << endl;
    cout << "2 Years Ago" << endl;
    Post* post = new Post("post11", "Never thought I will be specialist in this field…", "15/11/2015");
    Memory* memory = new Memory("post10", "Never thought I will be specialist in this field…", "15/11/2015", post);

    app.shareMemory("post10", "post11", "Never thought I will be specialist in this field…");
    app.getPost("post10")->viewDetails();

    cout << "---------------------------------------------------------------------------------------------------------------\n" << endl;
    cout << "Timeline with memory" << endl;

    app.viewTimeline("u7");
    cout << "---------------------------------------------------------------------------------------------------------------\n" << endl;
    cout << "command: viewPage" << endl;
    app.viewPage("P1");

    cout << "---------------------------------------------------------------------------------------------------------------\n" << endl;
    cout << "Command Set user to u11" << endl;
    app.setCurrentUser("u11");
    app.viewHome();
    cout << "---------------------------------------------------------------------------------------------------------------\n" << endl;
    cout << "Command: View Timeline" << endl;
    app.viewTimeline("u11");
}