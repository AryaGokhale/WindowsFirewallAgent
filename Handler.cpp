#include <string>
#include "handler.h"
using namespace std;


class PolicyInterface
{
public:
	virtual ~PolicyInterface() {};
	virtual void invokeRule() const = 0;
};

class AddAppRuleHandler : public PolicyInterface
{
public:
	void invokeRule() const override
	{
		runRule();
	}
};

class AddLANRuleHandler : public PolicyInterface
{
public:
	void invokeRule() const override
	{

	}
};

//PolicyHandler factory

class PolicyHandler
{
public:

	virtual ~PolicyHandler() {};
	virtual PolicyInterface* createHandler() const = 0;

	void handle() const
	{
		PolicyInterface* policy_handler = this->createHandler();
		policy_handler->invokeRule();
		delete policy_handler;
	};

};

class AppPolicyCreator : public PolicyHandler
{
public:
	PolicyInterface* createHandler() const override
	{
		return new AddAppRuleHandler();
	}
};

class LANPolicyCreator : public PolicyHandler
{
public:
	PolicyInterface* createHandler() const override
	{
		return new AddLANRuleHandler();
	}
};


void callHandler()
{
	string policyName = "AppPolicy";

	//printf("Enter policy name");

	//cin >> policyName;

	if (policyName == "AppPolicy")
	{
		PolicyHandler* policyhandler = new AppPolicyCreator();
		policyhandler->handle();
	}
	if (policyName == "LANPolicy")
	{
		PolicyHandler* policyhandler = new LANPolicyCreator();
		policyhandler->handle();
	}
}
