import * as React from "react";

import { SketchWrapper } from "@elisabethjoan/portfolio-scaffold";

import "./Modal.css";

export const Modal = ({ show, children }: { show: boolean; children: React.ReactElement }) => {
    const showHideClassName = show ? "display-block" : "display-none";
    return (
        <div className={showHideClassName}>
            <SketchWrapper>
                {children}
            </SketchWrapper>
        </div>
    )
}
