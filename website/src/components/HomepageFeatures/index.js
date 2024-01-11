import clsx from 'clsx';
import Heading from '@theme/Heading';
import styles from './styles.module.css';

const FeatureList = [
  {
    title: 'About this project',
    Svg: require('@site/static/img/about-this-project.svg').default,
    description: (
      <>
        The Open Smart Meter solves a problem that affects most solar developers in the industry.
        This inexpensive and open source Smart Meter meets IEC and key elements of the local regulations in Nigeria.
      </>
    ),
  },
  {
    title: 'Get Started',
    Svg: require('@site/static/img/get-started.svg').default,
    description: (
      <>
        This document summarizes the manufacturing guide to enable a skilled person (with software engineering knowledge) to manufacture the smart energy meter.

        Need some guidance with the Open Smart Meter? We can help!
      </>
    ),
  },
  {
    title: 'Project Summary',
    Svg: require('@site/static/img/summary.svg').default,
    description: (
      <>
        First Electric plans to maintain ownership of solar equipment and charge customers per kWh, based on exact consumption.
        For this solution to work effectively, they needed an inexpensive and OEM agnostic GSM prepaid smart energy meter.
      </>
    ),
  },
];

function Feature({ Svg, title, description }) {
  return (
    <div className={clsx('col col--4')}>
      <div className="text--center">
        <Svg className={styles.featureSvg} role="img" />
      </div>
      <div className="text--center padding-horiz--md">
        <Heading as="h3">{title}</Heading>
        <p>{description}</p>
      </div>
    </div>
  );
}

export default function HomepageFeatures() {
  return (
    <section className={styles.features}>
      <div className="container">
        <div className="row">
          {FeatureList.map((props, idx) => (
            <Feature key={idx} {...props} />
          ))}
        </div>
      </div>
    </section>
  );
}
