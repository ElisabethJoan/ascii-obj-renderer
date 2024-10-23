import * as React from "react";

import { Modal } from "./Modal";
import "./WASMLoader.css";

type WASMLoaderProps = {
    moduleName: string;
    args?: string[];
}

export const WASMLoader: React.FC<WASMLoaderProps> = ({ moduleName, args }) => {
    const [initialized, setInitialized] = React.useState(false);
    const canvasRef = React.useRef<HTMLCanvasElement>(null);
    
    function runModule() {
        const moduleMethod = (window as any)[moduleName];
        let module = {
            canvas: (() => canvasRef.current)(),
            print: (text: string) => console.log(text),
            printErr: (text: string) => console.log(text),
            arguments: args,
        };
        moduleMethod(module).then((finishModule: any) => {
            setInitialized(true);
            module = finishModule;
        })
    }

    function waitUntilModuleOnDOM() {
        if ((window as any)[moduleName]) {
            runModule();
        } else {
            setTimeout(waitUntilModuleOnDOM, 250)
        }
    }

    React.useEffect(() => {
        if ((window as any)[moduleName]) {
            runModule()
        } else {
            const script = document.createElement("script");
            script.src = moduleName+".js"
            document.body.appendChild(script);
            waitUntilModuleOnDOM();
        };
    }, [args]);

    return (
        <div>
            { !initialized && ( <div className="placeholderCanvas"></div> ) }
            <Modal show={initialized}>
                <canvas ref={canvasRef} className="canvas" id="canvas" onContextMenu={event => event.preventDefault()} />
            </Modal>
        </div>
    );
}
