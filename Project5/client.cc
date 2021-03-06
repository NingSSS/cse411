/**
 * CSE 411
 * Fall 2018
 * Programming Assignment #5 Part 1
 *
 * It is exceedingly rare for any real-world software to be written in a single
 * language, or to be completely self contained.  Modern software relies on
 * services provided by other software packages, and not just by way of linking
 * in libraries... modern software also tends to request services from on-line
 * servers, who expose Application Programming Interfaces (APIs) that govern how
 * they can be used.
 *
 * In this assignment, we will build a multi-language, multi-component program
 * that consumes a remote service.  There will be two C++ components, and an
 * HTML5 component.  These components will work together as a Twitter analytics
 * package.
 *
 * In more detail: at any time, the client program can be run.  It will take a
 * command line parameter, which is a search term, and it will issue a query
 * against the Twitter API.  It will process the result set as follows:
 *     Produce a histogram from the top 100 results, matching number of tweets
 *     to dates.  Save this in a MongoDB collection called "hist".  The query
 *     term should be the document name.
 * - Note: query terms that are user handles (@name) or hashtags (#tag) will
 *   naturally show user activity or topic trends
 * - Note: authentication for the Twitter API uses OAuth.  It will take some
 *   time for you to figure it out.  Plan accordingly.
 *
 * The server program should do three things.
 *   - It should serve an index.html file in response to GET /
 *   - It should serve other files in response to GET /files/<filename>
 *   - It should serve JSON, pulled from MongoDB, in response to GET /data/term
 * With regard to the third of these tasks, "term" should be a search term, and
 * the response should be a JSON representation of the histogram for that term,
 * as determined by the client.  If no histogram exists, then the JSON response
 * should be empty.
 *
 * The HTML5 component should be a single-page web app that allows the user to
 * enter a search term.  In response to that term, the page will issue an AJAX
 * request to the server to get some JSON data, and then it should use that JSON
 * data to initialize a chart.  You can use an existing JavaScript charting
 * library, such as Dygraphs, Chart.js, D3.js, etc.  You may also use other
 * JavaScript libraries (jQuery, Bootstrap, etc.), as you see fit.
 *
 * Your program should use Microsoft's cpprestsdk.  It should use advanced
 * features of C++ whenever appropriate.  It should compile with g++ version 7.
 * You should organize your code into files in a way that is appropriate.  You
 * should comment your code.  You should only use C++ libraries that are easily
 * installed into an Ubuntu Docker container.
 *
 * Turn-in will be via bitbucket.
 */

int main(int argc, char *argv[]) {
  // By now, you should be able to write your own argument parsing code :)
}