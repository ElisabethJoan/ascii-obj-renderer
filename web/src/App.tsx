// import { useState } from 'react'

import { WASMLoader } from "./WebAssemblyLoader"
import "./App.css"

function App() {

  return (
    <>
      <div>
        <WASMLoader moduleName={"main"}/>
      </div>
    </>
  )
}

export default App
