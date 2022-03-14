#define CROW_MAIN

#include <bits/stdc++.h>

#include "crow_all.h"

// Global data
struct GlobalData {
    GlobalData() : foo(0) {}
    int foo;
};
GlobalData gData;

int main(void) {
    crow::SimpleApp app;

    // Simple example: when querying the root endpoint /, just return some text
    CROW_ROUTE(app, "/")(
        [](){
            std::cerr << "Serving route /\n";
            return "hello world\nfoo=" + std::to_string(gData.foo);
        }
    );

    // More complicated example: when querying the /sys endpoint, return json
    // Allow query param "show" to only show certain data, e.g.,
    // /sys?show-cpu only shows cpu
    CROW_ROUTE(app, "/sys")(
        [](const crow::request& req){ // HTTP request object
            crow::json::wvalue resp;
            bool showCpu = true;
            bool showMem = true;
            bool showHost = true;
            const char* f = req.url_params.get("show");
            if (f) {
                std::string filter(f);
                if (filter == "cpu") {
                    showMem = false;
                    showHost = false;
                }
                else if (filter == "mem") {
                    showCpu = false;
                    showHost = false;
                }
                else if (filter == "host") {
                    showCpu = false;
                    showMem = false;
                }
                else {
                    resp["error"] = "invalid value for filter parameter: " + filter;
                    return resp;
                }
            }
            if (showCpu) resp["cpu"] = 64.5; // TODO
            if (showMem) resp["mem"] = 38.9; // TODO
            if (showHost) resp["host"] = "localhost"; // TODO
            return resp;
        }
    );


    // /foo ndpoint that supports post method to set the foo member in gData
    CROW_ROUTE(app, "/foo").methods("POST"_method)(
        [](const crow::request& req){ // HTTP request object
            auto body = crow::json::load(req.body);
            if (!body) {
                return crow::response(400); // crow::status::BAD_REQUEST (note: with crow_all.h somehow this is not defined)
            }
            int newFooVal = body["foo"].i();
            gData.foo = newFooVal;
            std::cerr << "set foo to " << newFooVal << "\n";
            return crow::response(200); // crow::status::OK
        }
    );


    std::cerr << "Server starting\n";
    app.port(18080).multithreaded().run();
    std::cerr << "Server shut down\n";

    return 0;
}
