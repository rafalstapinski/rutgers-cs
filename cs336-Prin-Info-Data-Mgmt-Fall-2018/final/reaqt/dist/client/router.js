"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const React = require("react");
const react_router_dom_1 = require("react-router-dom");
const not_found_1 = require("./pages/not-found/not-found");
const students_1 = require("./pages/students/students");
exports.Router = () => {
    return (React.createElement(react_router_dom_1.BrowserRouter, null,
        React.createElement("div", { className: 'container-fluid' },
            React.createElement(react_router_dom_1.Switch, null,
                React.createElement(react_router_dom_1.Route, { exact: true, path: '/', component: students_1.Students }),
                React.createElement(react_router_dom_1.Route, { exact: true, path: '/students', component: students_1.Students }),
                React.createElement(react_router_dom_1.Route, { path: '/', component: not_found_1.NotFound })))));
};
//# sourceMappingURL=router.js.map