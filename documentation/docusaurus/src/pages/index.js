import React from "react";

export default function Home() {
  React.useEffect(() => {
    window.location.href = "/docs/intro";
  }, []);
  return null;
}
