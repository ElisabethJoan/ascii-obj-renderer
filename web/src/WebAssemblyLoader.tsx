import * as React from "react";

import { Modal } from "./Modal";

type WASMLoaderProps = {
    moduleName: string,
}

export const WASMLoader: React.FC<WASMLoaderProps> = ({ moduleName }) => {
    const [initialized, setInitialized] = React.useState(false);
    const canvasRef = React.useRef<HTMLCanvasElement>(null);

    function runModule() {
        const moduleMethod = window[moduleName];
        let module = {
            canvas: (() => canvasRef.current)(),
            print: text => console.log(text),
            printErr: text => console.log(text),
        };
        moduleMethod(module).then((finishModule) => {
            setInitialized(true);
            module = finishModule;
        })
    }

    function waitUntilModuleOnDOM() {
        if (window[moduleName]) {
            runModule();
        } else {
            setTimeout(waitUntilModuleOnDOM, 250)
        }
    }

    React.useEffect(() => {
        console.log(1)
        if (window[moduleName]) {
            runModule()
        } else {
            const script = document.createElement("script");
            script.src = moduleName+".js"
            document.body.appendChild(script);
            waitUntilModuleOnDOM();
        };
    }, []);

    return (
        <div>
            <Modal show={initialized}>
                <canvas ref={canvasRef} className="canvas" id="canvas" onContextMenu={event => event.preventDefault()} />
            </Modal>
        </div>
    );
}
