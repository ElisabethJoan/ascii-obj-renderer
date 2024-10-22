import * as React from "react";

export const Modal = ({ show, children }: { show: boolean; children: React.ReactElement }) => {
    const showHideClassName = show ? "modal display-block" : "modal display-none";
    return (
        <div className={showHideClassName}>
            <section className="modal-main">
                {children}
            </section>
        </div>
    )
}
