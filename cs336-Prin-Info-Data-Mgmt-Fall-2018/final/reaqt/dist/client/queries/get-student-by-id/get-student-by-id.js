"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const React = require("react");
const react_apollo_1 = require("react-apollo");
const GET_STUDENT_BY_ID = require("./get-student-by-id.graphql");
exports.GetStudentById = id => (React.createElement(react_apollo_1.Query, { query: GET_STUDENT_BY_ID, variables: id }, ({ loading, error, data }) => {
    if (loading)
        return 'loading';
    if (error)
        return `Error! ${error.message}`;
    return (React.createElement("p", null,
        "id: ",
        data.getStudentById.id,
        " name: ",
        data.getStudentById.name));
}));
//# sourceMappingURL=get-student-by-id.js.map