"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const React = require("react");
const react_apollo_1 = require("react-apollo");
const GET_STUDENT_BY_NAME = require("./get-student-by-name.graphql");
exports.GetStudentByName = name => (React.createElement(react_apollo_1.Query, { query: GET_STUDENT_BY_NAME, variables: name }, ({ loading, error, data }) => {
    if (loading)
        return 'loading';
    if (error)
        return `Error! ${error.message}`;
    return (React.createElement("p", null,
        "id: ",
        data.getStudentByName.id,
        " name: ",
        data.getStudentByName.name));
}));
//# sourceMappingURL=get-student-by-name.js.map