// @ts-check
// `@type` JSDoc annotations allow editor autocompletion and type checking
// (when paired with `@ts-check`).
// There are various equivalent ways to declare your Docusaurus config.
// See: https://docusaurus.io/docs/api/docusaurus-config

import { themes as prismThemes } from "prism-react-renderer";
import remarkMath from "remark-math";
import rehypeKatex from "rehype-katex";

/** @type {import('@docusaurus/types').Config} */
const config = {
  title: "OpenSmartMeter",
  tagline:
    "Low-cost & OEM agnostic GSM prepaid smart energy meter with a customizable API.",
  favicon: "img/opensmartmeter-icon.svg",

  url: "https://enaccess.github.io/",
  // Set the /<baseUrl>/ pathname under which your site is served
  // For GitHub pages deployment, it is often '/<projectName>/'
  baseUrl: "/OpenSmartMeter/",

  // GitHub pages deployment config.
  // If you aren't using GitHub pages, you don't need these.
  organizationName: "EnAccess",
  projectName: "OpenSmartMeter",

  onBrokenLinks: "throw",
  onBrokenMarkdownLinks: "warn",

  // Even if you don't use internationalization, you can use this field to set
  // useful metadata like html lang. For example, if your site is Chinese, you
  // may want to replace "en" with "zh-Hans".
  i18n: {
    defaultLocale: "en",
    locales: ["en"],
  },

  markdown: {
    format: "detect",
    mermaid: true,
  },

  presets: [
    [
      "classic",
      /** @type {import('@docusaurus/preset-classic').Options} */
      ({
        docs: {
          path: "../docs",
          // Please change this to your repo. Remove this to remove the "edit this page" links.
          editUrl:
            "https://github.com/EnAccess/OpenSmartMeter/tree/main/shared/",
          remarkPlugins: [remarkMath],
          rehypePlugins: [rehypeKatex],
        },
        blog: {
          path: "../blog",
          showReadingTime: true,
          // Please change this to your repo. Remove this to remove the "edit this page" links.
          editUrl:
            "https://github.com/EnAccess/OpenSmartMeter/tree/main/shared/",
        },
        theme: {
          customCss: "./src/css/custom.css",
        },
        gtag: {
          trackingID: "G-PTQ3NKJGK5",
          anonymizeIP: true,
        },
      }),
    ],
  ],

  themes: [
    [
      require.resolve("@easyops-cn/docusaurus-search-local"),
      {
        hashed: true,
        // docsRouteBasePath: '/',
        highlightSearchTermsOnTargetPage: true,
      },
    ],
    "@docusaurus/theme-mermaid",
  ],

  stylesheets: [
    {
      href: "https://cdn.jsdelivr.net/npm/katex@0.13.24/dist/katex.min.css",
      type: "text/css",
      integrity:
        "sha384-odtC+0UGzzFL/6PNoE8rX/SPcQDXBJ+uRepguP4QkPCm2LBxH3FA3y+fKSiJ+AmM",
      crossorigin: "anonymous",
    },
  ],

  themeConfig:
    /** @type {import('@docusaurus/preset-classic').ThemeConfig} */
    ({
      image: "img/enaccess-logo.svg",
      navbar: {
        title: "OpenSmartMeter",
        logo: {
          alt: "EnAccess Logo",
          src: "img/enaccess-logo.svg",
          srcDark: "img/enaccess-logo-white.svg",
        },
        items: [
          {
            to: "/docs",
            label: "Docs",
            position: "left",
          },
          {
            to: "/docs/quick-start-guide",
            label: "Get Started",
            position: "left",
          },
          {
            to: "/blog",
            label: "Blog",
            position: "left",
          },
          {
            href: "https://github.com/EnAccess/OpenSmartMeter",
            position: "right",
            className: "header-github-link",
            "aria-label": "GitHub repository",
          },
        ],
      },
      footer: {
        style: "light",
        logo: {
          alt: "EnAccess Logo",
          src: "img/enaccess-logo.svg",
          srcDark: "img/enaccess-logo-white.svg",
          href: "https://enaccess.org",
          width: 160,
          height: 51,
        },
        links: [
          {
            title: "Docs",
            items: [
              {
                label: "Get Started 🚀",
                to: "/docs/quick-start-guide",
              },
              {
                label: "Read the documentation 📄",
                to: "/docs",
              },
            ],
          },
          {
            title: "Community",
            items: [
              {
                label: "Community",
                href: "https://community.enaccess.org/",
              },
              {
                label: "Discord (TBD)",
                href: "https://enaccess.org/",
              },
            ],
          },
          {
            title: "Follow us",
            items: [
              {
                label: "EnAccess Homepage",
                href: "https://enaccess.org/",
              },
              {
                label: "GitHub",
                href: "https://github.com/EnAccess/",
              },
              {
                label: "LinkedIn",
                href: "https://www.linkedin.com/company/enaccess/",
              },
              {
                label: "Youtube",
                href: "https://www.youtube.com/@EnAccessFoundation/",
              },
              {
                label: "Twitter",
                href: "https://twitter.com/EnAccessFdn/",
              },
            ],
          },
        ],
        copyright: `Copyright © ${new Date().getFullYear()} EnAccess.Built with Docusaurus ❤️.`,
      },
      prism: {
        theme: prismThemes.github,
        darkTheme: prismThemes.dracula,
        additionalLanguages: ["powershell", "php"],
      },
    }),
};

export default config;
