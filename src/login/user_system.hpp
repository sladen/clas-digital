/**
 * @file src/login/user_system.hpp
 *
 * This file defines the interface for the basic user class
 *
 */
#include <string>
#include <map>
#include <shared_mutex>
#include <memory>
#include "src/util/debug.hpp"
#ifdef COMPILE_UNITTEST
#include <gtest/gtest.h>
#endif

/**
 * @brief Defines the basic a access rights a user can have at the moment
 */
enum AccessRights
{
	USR_READ = 1,	///< The user has read access means, he can access all books for reading
	USR_WRITE = 2,	///< The user has write access he upload new books and change existing ones
	USR_ADMIN = 4	///< The user is an admin he can create new users and give all users new rights, he can delete users as well
};

/**
 * @brief The basic user class this represents a basic user and stores email password and access rights for this user
 *
 */
class User
{
	private:
		std::string _password; 		///<The user password.
		std::string _email;			///<The email of the user.
		volatile int _accessRights;	///<The access rights the user has.
		std::string _sessid;

	public:
		/**
		 * Default constructor for the User class.
		 */
		User();

		/**
		 * Constructs a user with a given email, password and access rights.
		 * @param email The email the user got
		 * @param pass	The password the user will use to login
		 * @param access The access rights the user got.
		 */
		User(const char *email, const char *pass, int access);


		/**
		 * Returns the user information as json file.
		 * User information means: email and access rights.
		 * @return A string in the json format containing email and access rights of the user
		 */
		std::string toJSON() const;

		/**
		 * Getter for the access rights of the user.
		 */
		int GetAccessRights() const;

		/**
		 * Setter for the access rights of the user.
		 * @param acc The new access rights for the user.
		 */
		void SetAccessRights(int acc); 

		/**
		 * The getter for the email the user uses.
		 */
		const std::string &GetEmail() const;

		/**
		 * The getter for the password of the user.
		 */
		const std::string &GetPassword() const;

		const std::string &GetSessid() const;

		void SetSessionId(std::string sessid);

		/**
		 * Checks if the given password and email matches the users credentials.
		 * @param email The email to check against
		 * @param passwd The Password to check against
		 * @return Returns true if the given credentials matches the user credentials
		 */
		bool DoesMatch(std::string email, std::string passwd) const;
};

/**
 * @brief The user handler got a list of all available users and manages creating and deleting new users
 */
class UserHandler
{
	private:
		std::map<std::string,std::shared_ptr<User>> m_userTable; 	///< Maps all user based on their email to their user account
		std::shared_mutex m;											///< Used for global locking of the user map

	public:
		/**
		 * @brief Loads the user table from the specified path and initialises it
		 * @param filePath The path to the saved user table
		 */
		UserHandler(std::string filePath);

		/**
		 * Adds a user to the map of current users
		 */
		bool AddUser(std::string email, std::string password, int access);

		/**
		 * Set the access rights for a specific user and save the changes instantly to disk.
		 * @param email The email of the user who gets the access rights changed
		 * @param acc The new access rights the user gets granted
		 */
		void SetAccessRights(std::string email, int newAccess);

		/**
		 * Converts the complete user table to a string formatted in json style.
		 * The json contains only email name and access rights.
		 * @return The string containing the UserTable in json format
		 */
		std::string toJSON();

		/**
		 * Removes a user from the user table and deletes all files and all folder associated with him.
		 * @param usr The user which should be removed from the user table
		 */
		void RemoveUser(std::string email);

		/**
		 * Checks if a given set of email and password matches an existing user and returns
		 * the user if the password and login matches.
		 * @param email The email address of the user.
		 * @param password The Password of the user
		 * @return returns either the user if the password/email matches an user or zero if there is no
		 * user with this password and/or email.
		 */
		std::string DoLogin(std::string email, std::string password);
		std::shared_ptr<User> GetUserBySessid(std::string x);
		void RemoveSession(std::string x);
};
