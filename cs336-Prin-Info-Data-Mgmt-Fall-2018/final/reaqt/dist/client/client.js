"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const React = require("react");
const ReactDOM = require("react-dom");
const router_1 = require("./router");
const apollo_client_1 = require("apollo-client");
const apollo_link_http_1 = require("apollo-link-http");
const apollo_cache_inmemory_1 = require("apollo-cache-inmemory");
const react_apollo_1 = require("react-apollo");
const WrappedRouter = () => {
    const apolloClient = new apollo_client_1.ApolloClient({
        link: new apollo_link_http_1.HttpLink(),
        cache: new apollo_cache_inmemory_1.InMemoryCache()
    });
    return (React.createElement(react_apollo_1.ApolloProvider, { client: apolloClient },
        React.createElement(router_1.Router, null)));
};
ReactDOM.render(React.createElement(WrappedRouter, null), document.getElementById('app'));
//# sourceMappingURL=client.js.map