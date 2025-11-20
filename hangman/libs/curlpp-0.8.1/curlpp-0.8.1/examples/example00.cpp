/**
 * \file
 * The most simple example.
 *
 */

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>

using namespace curlpp::options;

int main(int, char **)
{
	try
	{
		// That's all that is needed to do cleanup of used resources (RAII style).
		curlpp::Cleanup myCleanup;

		// Our request to be sent.
		curlpp::Easy myRequest;

		// Set the URL.
		myRequest.setOpt<Url>("http://example.com");

		// Send request and get a result.
		// By default the result goes to standard output.
		myRequest.perform();
	}

	catch (curlpp::RuntimeError &e)
	{
		cout << e.what() << endl;
	}

	catch (curlpp::LogicError &e)
	{
		cout << e.what() << endl;
	}

	return 0;
}
