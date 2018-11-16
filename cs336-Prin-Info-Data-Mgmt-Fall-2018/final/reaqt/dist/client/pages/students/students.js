"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const React = require("react");
const get_student_by_id_1 = require("../../queries/get-student-by-id/get-student-by-id");
const get_student_by_name_1 = require("../../queries/get-student-by-name/get-student-by-name");
exports.Students = () => {
    return (React.createElement(React.Fragment, null,
        React.createElement(get_student_by_id_1.GetStudentById, { id: '1' }),
        React.createElement(get_student_by_name_1.GetStudentByName, { name: 'raf' })));
};
//# sourceMappingURL=students.js.map